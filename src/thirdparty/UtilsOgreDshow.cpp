
// Ogre Dshow: small wrapper for video reproduction in Ogre, using Direct Show 9.
/*
Wrapper for video reproduction using Direct Show in the Ogre 3d engine.

Coded by H. Hernán Moraldo from Moraldo Games
www.hernan.moraldo.com.ar/pmenglish/field.php

--------------------

Copyright (c) 2007 Horacio Hernan Moraldo

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held liable
for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:
. The origin of this software must not be misrepresented; you
must not claim that you wrote the original software. If you use
this software in a product, an acknowledgment in the product
documentation would be appreciated but is not required.
. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.
. This notice may not be removed or altered from any source
distribution.

*/
//#include "stdafx.h"
#include "UtilsOgreDshow.h"
#include "UtilsOgreDshow_private.h"
#include <OgreStringConverter.h>
#include <dshow.h>
#include <iostream>
template<> OgreUtils::DirectShowManager *Ogre::Singleton<OgreUtils::DirectShowManager>::ms_Singleton=NULL;

namespace OgreUtils
{
    DirectShowMovieTexture::DirectShowMovieTexture(int width, int height, bool dontModifyDimensions)
    {
        // 1) CREATE DSDATA
        dsdata=new DirectShowData;

        // 2) CREATE TEXTURE
        // get width and height to the next square of two
        int twoSquared;
        mTexWidth=0; mTexHeight=0;
        for (twoSquared=2; mTexWidth==0 || mTexHeight==0; twoSquared*=2)
        {
            if (mTexWidth==0 && twoSquared>=width)
                mTexWidth=twoSquared;
            if (mTexHeight==0 && twoSquared>=height)
                mTexHeight=twoSquared;
        }
        if (dontModifyDimensions)
        {
            // back to the original dimensions
            mTexWidth=width;
            mTexHeight=height;
        }

        // log it
        Ogre::LogManager::getSingletonPtr()->logMessage(
            Ogre::String("[DSHOW] Creating texture with dimensions ")+
            Ogre::StringConverter::toString(mTexWidth)+"x"+
            Ogre::StringConverter::toString(mTexHeight)+".");

        // first, create the texture we are going to use
        /*
            mTexture=Ogre::TextureManager::getSingleton().createManual(
                        "DirectShowManualTexture",// name
                        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                        Ogre::TEX_TYPE_2D,// texture type
                        mTexWidth,
                        mTexHeight,
,// number of mipmaps
                        Ogre::PF_BYTE_BGRA,// pixel format
                        Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE// usage
                        );*/
            

        // 3) INITIALIZE DIRECT SHOW
        HRESULT hr;

        hr=CoInitialize(NULL);
        if (FAILED(hr)) throw("[DSHOW] Error in co initialize");

        // initialize all pointers
        dsdata->pGraph=0;
        dsdata->pControl=0;
        dsdata->pEvent=0;
        dsdata->pGrabberF=0;
        dsdata->pGrabber=0;
        dsdata->pSeeking=0;
        dsdata->pWindow=0;


    }

    DirectShowMovieTexture::~DirectShowMovieTexture()
    {
        // 1) DEINITIALIZE DIRECT SHOW
        unloadMovie();
        CoUninitialize();

        // 2) DESTROY TEXTURE
        Ogre::TextureManager::getSingleton().remove(mTexture->getName());

        // 3) DELETE DSDATA
        delete dsdata;
    }

    void DirectShowMovieTexture::loadMovie(
        const Ogre::String& moviePath, bool horizontalMirroring)
    {
        HRESULT hr;

        // log it!
        Ogre::LogManager::getSingletonPtr()->logMessage(
            Ogre::String("[DSHOW] Loading movie named '")+
            moviePath+"'.");

        // destroy previous movie objects (if any)
        unloadMovie();

        // create filter graph and get interfaces
        hr=CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
            IID_IGraphBuilder, (void**) &dsdata->pGraph);
        if (FAILED(hr)) throw("[DSHOW] Error in creating graph");

        hr=dsdata->pGraph->QueryInterface(IID_IMediaControl, (void**) & dsdata->pControl);
        if (FAILED(hr)) throw("[DSHOW] Error in querying media control");

        hr=dsdata->pGraph->QueryInterface(IID_IMediaEvent, (void**) & dsdata->pEvent);
        if (FAILED(hr)) throw("[DSHOW] Error in querying media event");

        hr=dsdata->pGraph->QueryInterface(IID_IMediaSeeking, (void**) & dsdata->pSeeking);
        if (FAILED(hr)) throw("[DSHOW] Error in querying seeking interface");

        // create sample grabber
        hr=CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,
            IID_IBaseFilter, (void**)&dsdata->pGrabberF);
        if (FAILED(hr)) throw("[DSHOW] Error in creating sample grabber");

        // add sample grabber to the graph
        hr=dsdata->pGraph->AddFilter(dsdata->pGrabberF, L"Sample Grabber");
        if (FAILED(hr)) throw("[DSHOW] Error in adding sample grabber to the graph");

        // get sample grabber object
        dsdata->pGrabberF->QueryInterface(IID_ISampleGrabber,
            (void**)&dsdata->pGrabber);

        // set sample grabber media type
        AM_MEDIA_TYPE mt;
        ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
        mt.majortype = MEDIATYPE_Video;
        mt.subtype = MEDIASUBTYPE_RGB24;
        mt.formattype = FORMAT_VideoInfo;
        hr=dsdata->pGrabber->SetMediaType(&mt);
        if (FAILED(hr)) throw("[DSHOW] Error in setting sample grabber media type");

        // open the file!
        WCHAR* filepath=util_convertCStringToWString(moviePath.c_str());
		//std::cout << filepath << std::endl;
        hr=dsdata->pGraph->RenderFile(filepath, NULL);
		std::cout << hr << std::endl;
        if (FAILED(hr)) throw("[DSHOW] Error opening video file!");

        // disable auto show
        // (wouldn't be needed if we used the null renderer)
        hr=dsdata->pGraph->QueryInterface(IID_IVideoWindow, (void**) & dsdata->pWindow);
        if (FAILED(hr)) throw("[DSHOW] Error getting video window interface");
        dsdata->pWindow->put_AutoShow(OAFALSE);

        // get video information
        AM_MEDIA_TYPE mtt;
        hr=dsdata->pGrabber->GetConnectedMediaType(&mtt);

        if (FAILED(hr)) throw("[DSHOW] Error getting connected media type info");

        VIDEOINFOHEADER *vih = (VIDEOINFOHEADER*) mtt.pbFormat;
        dsdata->videoWidth=vih->bmiHeader.biWidth;
        dsdata->videoHeight=vih->bmiHeader.biHeight;
 
        
        ResetSize(dsdata->videoWidth,dsdata->videoHeight);
        //纹理的创建现在修改到了这里,原来在构造函数中
        //放在这里可以根据视频的大小来生成纹理的大小
        //这样可以让视频完全平铺到面片上
        mTexture=Ogre::TextureManager::getSingleton().createManual(
            "DirectShowManualTexture",// name
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            Ogre::TEX_TYPE_2D,// texture type
            mTexWidth,
            mTexHeight,
			0,// number of mipmaps
            Ogre::PF_BYTE_BGRA,// pixel format
            Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE// usage
            );
 
        // microsoft's help version of free media type
        if (mtt.cbFormat != 0)
        {
            CoTaskMemFree((PVOID)mtt.pbFormat);
            mtt.cbFormat = 0;
            mtt.pbFormat = NULL;
        }
        if (mtt.pUnk != NULL)
        {
            mtt.pUnk->Release();
            mtt.pUnk = NULL;
        }

        // log it
        Ogre::LogManager::getSingletonPtr()->logMessage(
            Ogre::String("[DSHOW] -> This movie has dimensions: ")+
            Ogre::StringConverter::toString(dsdata->videoWidth)+"x"+
            Ogre::StringConverter::toString(dsdata->videoHeight)+".");


        // set sampling options
        dsdata->pGrabber->SetOneShot(FALSE);
        dsdata->pGrabber->SetBufferSamples(TRUE);

        // set some basic data
        mHorizontalMirroring=horizontalMirroring;

        // clean the texture, so that it's ready for rendering this video
        cleanTextureContents();
    }

    Ogre::Vector2 DirectShowMovieTexture::getMovieDimensions()
    {
        return Ogre::Vector2(dsdata->videoWidth, dsdata->videoHeight);
    }

    void DirectShowMovieTexture::unloadMovie()
    {
        if (dsdata->pGraph==0)
            return;

        if (dsdata->pGrabber!=0)
        {
            dsdata->pGrabber->Release();
            dsdata->pGrabber=0;
        }
        if (dsdata->pGrabberF!=0)
        {
            dsdata->pGrabberF->Release();
            dsdata->pGrabberF=0;
        }
        if (dsdata->pWindow!=0)
        {
            dsdata->pWindow->Release();
            dsdata->pWindow=0;
        }
        if (dsdata->pSeeking!=0)
        {
            dsdata->pSeeking->Release();
            dsdata->pSeeking=0;
        }
        if (dsdata->pControl!=0)
        {
            dsdata->pControl->Release();
            dsdata->pControl=0;
        }
        if (dsdata->pEvent!=0)
        {
            dsdata->pEvent->Release();
            dsdata->pEvent=0;
        }
        if (dsdata->pGraph!=0)
        {
            dsdata->pGraph->Release();
            dsdata->pGraph=0;
        }

    }

    void DirectShowMovieTexture::pauseMovie()
    {
        // pause!
        if (dsdata->pControl)
            dsdata->pControl->Pause();
    }

    void DirectShowMovieTexture::playMovie()
    {
        // play!
        if (dsdata->pControl)
            dsdata->pControl->Run();
    }

    void DirectShowMovieTexture::rewindMovie()
    {
        if (!dsdata->pSeeking) return;

        // rewind!
        LONGLONG p1=0;
        LONGLONG p2=0;

        dsdata->pSeeking->SetPositions(
            &p1, AM_SEEKING_AbsolutePositioning, &p2, AM_SEEKING_NoPositioning);
    }

    void DirectShowMovieTexture::stopMovie()
    {
        // stop!
        if (dsdata->pControl)
            dsdata->pControl->Stop();
    }


    Ogre::TexturePtr DirectShowMovieTexture::getMovieTexture()
    {
        return mTexture;
    }

    void DirectShowMovieTexture::updateMovieTexture()
    {
        HRESULT hr;
        unsigned int i, idx;
        int x, y;
        BYTE* bmpTmp;

        // only do this if there is a graph that has been set up
        if (!dsdata->pGraph)
            return;

        // Find the required buffer size.
        long cbBuffer = 0;
        hr = dsdata->pGrabber->GetCurrentBuffer(&cbBuffer, NULL);
        if (cbBuffer<=0)
        {
            // nothing to do here yet
            return;
        }

        char *pBuffer = new char[cbBuffer];
        if (!pBuffer) 
        {
            // out of memory!
            throw("[DSHOW] Out of memory or empty buffer");
        }
        hr = dsdata->pGrabber->GetCurrentBuffer(&cbBuffer, (long*)pBuffer);
        if (hr==E_INVALIDARG || hr==VFW_E_NOT_CONNECTED || hr==VFW_E_WRONG_STATE)
        {
            // we aren't buffering samples yet, do nothing
            delete[] pBuffer;
            return;
        }
        if (FAILED(hr)) throw("[DSHOW] Failed at GetCurrentBuffer!");

        // OGRE BEGIN
        // OGRE TEXTURE LOCK
        // get the texture pixel buffer
        int texw=mTexture->getWidth();
        int texh=mTexture->getHeight();
        Ogre::HardwarePixelBufferSharedPtr pixelBuffer = mTexture->getBuffer();
        bmpTmp=(BYTE*)pBuffer;

        // lock the pixel buffer and get a pixel box
        pixelBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
        const Ogre::PixelBox& pixelBox = pixelBuffer->getCurrentLock();

        Ogre::uint8* pDest = static_cast<Ogre::uint8*>(pixelBox.data);

        // FILL!
        // check for mirroring...
        bool shouldBeMirrored=mHorizontalMirroring;
        if (shouldBeMirrored){
            x=dsdata->videoWidth-1; y=dsdata->videoHeight-1;
        }else{
            x=0; y=dsdata->videoHeight-1;
        }

        // go set all bits...
        for (i=0; i<(dsdata->videoWidth*dsdata->videoHeight*3); i+=3){
            idx=(x*4)+y*pixelBox.rowPitch*4;

            // paint
            pDest[idx]=bmpTmp[i];//b
            pDest[idx+1]=bmpTmp[i+1];//g
            pDest[idx+2]=bmpTmp[i+2];//r
            pDest[idx+3]=255;//a

            if (shouldBeMirrored){
                x--;
                if (x<0){
                    x=dsdata->videoWidth-1;
                    y--; if (y<0) y=0;
                }
            }else{
                x++;
                if (x>=dsdata->videoWidth){
                    x=0;
                    y--; if (y<0) y=0;
                }
            }
        }

        // UNLOCK EVERYTHING!
        // unlock the pixel buffer
        pixelBuffer->unlock();
        // OGRE END

        // bye
        delete[] pBuffer;
    }

    void DirectShowMovieTexture::cleanTextureContents()
    {
        unsigned int idx;
        int x, y;

        // OGRE TEXTURE LOCK
        // get the texture pixel buffer
        int texw=mTexture->getWidth();
        int texh=mTexture->getHeight();
        Ogre::HardwarePixelBufferSharedPtr pixelBuffer = mTexture->getBuffer();

        // lock the pixel buffer and get a pixel box
        pixelBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
        const Ogre::PixelBox& pixelBox = pixelBuffer->getCurrentLock();

        Ogre::uint8* pDest = static_cast<Ogre::uint8*>(pixelBox.data);

        // FILL!
        for (x=0, y=0; y<texh; ){
            idx=(x*4)+y*pixelBox.rowPitch*4;

            // paint
            pDest[idx]=0;//b
            pDest[idx+1]=0;//g
            pDest[idx+2]=0;//r
            pDest[idx+3]=255;//a

            x++;
            if (x>=texw)
            {
                x=0;
                y++;
            }
        }

        // UNLOCK EVERYTHING!
        // unlock the pixel buffer
        pixelBuffer->unlock();
        // OGRE END
    }


    bool DirectShowMovieTexture::isPlayingMovie()
    {
        OAFilterState pfs;
        HRESULT hr;

        if (dsdata->pEvent!=NULL){
            long ev, p1, p2;

             
            while (E_ABORT!=dsdata->pEvent->GetEvent(&ev, &p1, &p2, 0)){
                // check for completion
                if (ev==EC_COMPLETE)
                {
                    pauseMovie();
                    return false;
                }

                // release event params
                hr=dsdata->pEvent->FreeEventParams(ev, p1, p2);
                if (FAILED(hr))
                {
                    pauseMovie();
                    return false;
                }
            }
        }

        // get the running state!
        if (dsdata->pControl!=NULL)
        {
            hr=dsdata->pControl->GetState(0, &pfs);
            if (FAILED(hr))
            {
                pauseMovie();
                return false;
            }

            return pfs==State_Running;
        }

        // it hasn't even been initialized!
        return false;
    }

    WCHAR* util_convertCStringToWString(const char* string)
    {
        const int MAX_STRINGZ=500;
        static WCHAR wtext[MAX_STRINGZ+2];

        if (strlen(string)>MAX_STRINGZ)
            return 0;

        // convert text to wchar
        if (MultiByteToWideChar(
            CP_ACP,// ansi code page
			0,// flags
            string,// orig string
            -1,// calculate len
            wtext,// where to put the string
            MAX_STRINGZ)// maximum allowed path
            ==0)
        {
            throw("[DSHOW] convertCStringToWString failed with no extra error info");
        }

        return wtext;
    }


 
    DirectShowControl::DirectShowControl(Ogre::String name,Ogre::String filename, int width,int height,Ogre::Viewport *vp,bool overlay/*=true*/ ):isOverlay(overlay),
        mName(name),mWidth(width),mHeight(height),mFilename(filename),mVp(vp)
    {
        mDirectshowTexture = new DirectShowMovieTexture(mWidth,mHeight,false);
        mDirectshowTexture->loadMovie(mFilename);
        
        createMaterial();
         
        if(isOverlay)
            createOverlay();
         
            
    }

    DirectShowControl::~DirectShowControl()
    {

    }
    void DirectShowControl::createOverlay()
    {
        mOverlay = Ogre::OverlayManager::getSingleton().create(mName);

        Ogre::OverlayContainer *mContainer = (Ogre::OverlayContainer*)
            (Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", "Ogre/DebugTexPanel" +mName));

        mContainer->setMetricsMode(Ogre::GMM_PIXELS);
        mContainer->setDimensions(mVp->getActualWidth(),mVp->getActualHeight());
        mContainer->setMaterialName(mMaterial->getName());
        mOverlay->add2D(mContainer);
        mOverlay->show();
        PlayMovie();
    }

    void DirectShowControl::createMaterial()
    {
        Ogre::String MatName=mName+"_directShowMaterial";
        mMaterial=Ogre::MaterialManager::getSingleton().create(MatName,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        //创建纹理对象并设置参数
        Ogre::String texName = mName+"_DirectshowTexture";
        mMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(1,1,1);
        Ogre::TextureUnitState *tex= mMaterial->getTechnique(0)->getPass(0)->createTextureUnitState( texName );
        tex->setTextureFiltering(Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_NONE);

        //获得纹理空间并赋上值
        tex=mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0);
        tex->setTextureName(
            mDirectshowTexture->getMovieTexture()->getName());

        PlayMovie();
    }

     
    Ogre::MaterialPtr DirectShowControl::getMaterial()
    {
        return mMaterial;
  
    }

    void DirectShowControl::Destroy()
    {
         
        delete mDirectshowTexture;
    }

    void DirectShowControl::PlayMovie()
    {
        if(mDirectshowTexture)
            mDirectshowTexture->playMovie();
    }

    void DirectShowControl::StopMovie()
    {
        if(mDirectshowTexture)
            mDirectshowTexture->stopMovie();
    }

    

    DirectShowManager::DirectShowManager(Ogre::Viewport *vp):mVP(vp)
    {

    }
    DirectShowManager::~DirectShowManager()
    {

    }
     

    DirectShowControl * DirectShowManager::createDirectshowControl(Ogre::String name,Ogre::String filename,int VWidth,int VHeight,bool overlay/*=true*/ )
    {
        DirectShowControl *mContrl=new DirectShowControl(name,filename,VWidth,VHeight,mVP,overlay);

        mDirectCtrlList.push_back(mContrl);

        return mContrl;
    }

    bool DirectShowManager::frameStarted( const Ogre::FrameEvent& evt )
    {

        DirectShowControlList::iterator it;
        for (it=mDirectCtrlList.begin();it!=mDirectCtrlList.end();++it)
        {
            DirectShowControl *con=*(it);
            if(con)
            {
                con->mDirectshowTexture->updateMovieTexture();
                
                if(!con->mDirectshowTexture->isPlayingMovie())//循环放
                    con->mDirectshowTexture->rewindMovie();

            }
        }

        return true;
    }

    void DirectShowManager::DestroyAll()
    {
        DirectShowControlList::iterator it;
        for (it=mDirectCtrlList.begin();it!=mDirectCtrlList.end();++it)
        {
            DirectShowControl *con=*(it);
            if(con)
                con->Destroy();
        }
        mDirectCtrlList.clear();
    }

    
}