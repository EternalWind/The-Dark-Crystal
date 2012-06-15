
/// Do not include this file directly, always use UtilsOgreDshow.h instead.
// Ogre Dshow: small wrapper for video reproduction in Ogre, using Direct Show 9.
/*
Wrapper for video reproduction using Direct Show in the Ogre 3d engine.

Coded by H. Hern¨¢n Moraldo from Moraldo Games
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

#ifndef __FILE_UTILSOGREDSHOW_PRIVATE_INCLUDED
#define __FILE_UTILSOGREDSHOW_PRIVATE_INCLUDED

#pragma include_alias( "dxtrans.h", "qedit.h" )
#define __IDxtCompositor_INTERFACE_DEFINED__
#define __IDxtAlphaSetter_INTERFACE_DEFINED__
#define __IDxtJpeg_INTERFACE_DEFINED__
#define __IDxtKey_INTERFACE_DEFINED__
#include <dshow.h>
#include <qedit.h>// for sample grabber
#include <windows.h>


namespace OgreUtils
{
    struct DirectShowData
    {
        /// Graph object
        IGraphBuilder *pGraph;
        /// Media control object
        IMediaControl *pControl;
        /// Media event object
        IMediaEvent *pEvent;
        /// Grabber filter
        IBaseFilter *pGrabberF;
        /// Grabber object
        ISampleGrabber *pGrabber;
        /// Interface for seeking object
        IMediaSeeking *pSeeking;
        /// Window interface
        /** Useful for some configuration
        */
        IVideoWindow *pWindow;

        /// Video output width
        int videoWidth;
        /// Video output height
        int videoHeight;
    };

    /// Util function for converting C strings to wide strings
    /** (as needed for path in directshow). */
    WCHAR* util_convertCStringToWString(const char* string);
}


#endif // __FILE_UTILSOGREDSHOW_PRIVATE_INCLUDED