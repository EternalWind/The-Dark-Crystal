#include "SceneLoader.h"

#include "Graphics/CameraComponent.hpp"
#include "Graphics/MeshComponent.hpp"
#include "Graphics/LightComponent.hpp"
#include "Audio/SoundComponent.hpp"
#include "Audio/MusicComponent.hpp"
#include "Logic/ScriptComponent.hpp"
#include "Logic/InteractionComponent.hpp"
#include "Logic/RaycastComponent.hpp"
#include "Logic/CollisionComponent.hpp"
#include "Logic/TriggerAreaComponent.hpp"
#include "Core/ResourceManager.hpp"
#include "Alien.h"
#include "Ammo.h"
#include "Monster.h"
#include "Weapon.h"
#include "HumanAgent.h"
#include "MonsterAIAgent.h"
#include "PlayerAIAgent.h"
#include "FirstAidKit.h"
#include "Vehicle.h"
#include "Entity.h"
#include "Crystal.h"
#include "Spaceship.h"
#include "Agent.h"
#include "EntityManager.h"
#include "AIDivideAreaManager.h"
#include "PlayerAIAgent.h"
#include <OgreProcedural.h>
#include <OgreSubEntity.h>

#include <cstdint>

Scene* SceneLoader::mScene = nullptr;

Scene* SceneLoader::loadScene(QString path)
{
    mScene = nullptr;
    QFile file(path);
    QDomDocument doc;
    if ( !file.open(QIODevice::ReadOnly) )
    {
        dt::Logger::get().error("Couldn't open file " + path);
        return nullptr;
    }

    cout << "loading result: " << endl;
    if ( doc.setContent(&file) )
    {
        mScene = new Scene(path);
        OgreProcedural::Root::getInstance()->sceneManager = mScene->getSceneManager();

        QDomElement root = doc.documentElement();

        for ( QDomElement scene_child = root.firstChildElement();
            !scene_child.isNull(); scene_child = scene_child.nextSiblingElement() )
        {
            if ( scene_child.nodeName() != SL_NODES ) //For free components(not including free mesh components).
            {
                __loadElement(scene_child);
            }
            else //For nodes and free mesh components.
            {
                for ( QDomElement nodes_child = scene_child.firstChildElement();
                    !nodes_child.isNull(); nodes_child = nodes_child.nextSiblingElement() )
                {
                    __loadElement(nodes_child);
                }
            }
        }
    }
    AIDivideAreaManager::get()->afterLoadScene();
    return mScene;
}

Node::NodeSP SceneLoader::__loadElement(const QDomElement& og_element, Node::NodeSP dt_node)
{    
    QString name = og_element.nodeName();
    Node::NodeSP node = nullptr;

    if ( name == SL_LIGHT )
    {
        node = __loadLight(og_element, dt_node);                   //Light
    }
    else if ( name == SL_CAMERA )
    {
        node = __loadCamera(og_element, dt_node);                  //Camera
    }
    else if ( name == SL_SOUND )
    {
        node = __loadSound(og_element, dt_node);                   //Sound
    }
    else if ( name == SL_MUSIC )
    {
        node = __loadMusic(og_element, dt_node);                   //Music
    }
    else if ( name == SL_SCPATH )
    {
        node = __loadScriptPath(og_element, dt_node);              //ScriptPath
    }
    else if ( name == SL_INTERACTOR )
    {
        node = __loadInteractor(og_element, dt_node);              //Interactor
    }
    else if ( name == SL_PHYSICS )
    {
        node = __loadPhysics(og_element, dt_node);                 //Physics
    }
    else if ( name == SL_CONTROLLER )
    {
        node = __loadController(og_element, dt_node);                 //Controller
    }
    else if ( name == SL_TRIGGER )
    {
        node = __loadTriggerArea(og_element, dt_node);                 //TriggerArea
    }
    else if (name == SL_ALIEN)
    {
        node = __loadAlien(og_element, dt_node);
    }
    else if (name == SL_AMMO)
    {
        node = __loadAmmo(og_element, dt_node);
    }
    else if (name == SL_CRYSTAL)
    {
        node = __loadAmmo(og_element, dt_node);
    }
    else if (name == SL_FIRSTAIDKIT)
    {
        node = __loadFirstAidKit(og_element, dt_node);
    }
    else if (name == SL_MONSTER)
    {
        node = __loadMonster(og_element, dt_node);
    }
    else if (name == SL_WEAPON)
    {
        node = __loadWeapon(og_element, dt_node);
    }
    else if (name == SL_SPACESHIP)
    {
        node = __loadSpaceship(og_element, dt_node);
    } 
    else if (name == SL_POINT)
    {
        node = __loadPoint(og_element, dt_node);
    }
    else if (name == SL_EDGE)
    {
        node = __loadEdge(og_element, dt_node);
    }
    else if ( name == SL_NODE )
    {
        if ( og_element.firstChildElement(SL_MESH_ENTITY).isNull() && og_element.firstChildElement(SL_MESH_PLANE).isNull() )
        {
            node = __loadNode(og_element, dt_node);                //Node
        }
        else
        {
            node = __loadMesh(og_element, dt_node);                //Mesh
        }
    }

    return node;
}

Node::NodeSP SceneLoader::__loadNode(const QDomElement& og_node, Node::NodeSP dt_parent)
{
    Node::NodeSP node = nullptr;

    if ( !og_node.isNull() )
    {
        if ( dt_parent )
        {
            node = dt_parent->addChildNode(new Node(og_node.attribute(SL_NAME)));
        }
        else
        {
            node = mScene->addChildNode(new Node(og_node.attribute(SL_NAME)));
        }

        QDomElement pos = og_node.firstChildElement(SL_POS);
        QDomElement rot = og_node.firstChildElement(SL_ROT);
        QDomElement scale = og_node.firstChildElement(SL_SCALE);

        node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
            pos.attribute(SL_Z).toFloat());
        node->setRotation(Ogre::Quaternion(rot.attribute(SL_QW).toFloat(),
            rot.attribute(SL_QX).toFloat(), rot.attribute(SL_QY).toFloat(), rot.attribute(SL_QZ).toFloat()));
        node->setScale(Ogre::Vector3(scale.attribute(SL_X).toFloat(), scale.attribute(SL_Y).toFloat(),
            scale.attribute(SL_Z).toFloat()));

        for ( QDomElement node_child = scale.nextSiblingElement();
            !node_child.isNull(); node_child = node_child.nextSiblingElement() )
        {
            __loadElement(node_child, node);
        }

    }

    return node;
}

Node::NodeSP SceneLoader::__loadMesh(const QDomElement& og_component, Node::NodeSP dt_node)
{
    Node::NodeSP node = dt_node;

    if ( !og_component.isNull() )
    {
        QString name = og_component.attribute(SL_NAME);

        if ( node == nullptr )
        {
            node = mScene->addChildNode(new Node(name + "_node"));

            QDomElement pos = og_component.firstChildElement(SL_POS);
            QDomElement rot = og_component.firstChildElement(SL_ROT);
            QDomElement scale = og_component.firstChildElement(SL_SCALE);

            node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
                pos.attribute(SL_Z).toFloat());
            node->setRotation(Ogre::Quaternion(rot.attribute(SL_QW).toFloat(),
                rot.attribute(SL_QX).toFloat(), rot.attribute(SL_QY).toFloat(), rot.attribute(SL_QZ).toFloat()));
            node->setScale(Ogre::Vector3(scale.attribute(SL_X).toFloat(), scale.attribute(SL_Y).toFloat(),
                scale.attribute(SL_Z).toFloat()));
        }

        QDomElement unknown_mesh = og_component.firstChildElement(SL_SCALE).nextSiblingElement();
        if ( unknown_mesh.nodeName() == SL_MESH_ENTITY )
        {
            const QDomElement& entity = unknown_mesh;

            //add mesh component
            auto mesh = node->addComponent<MeshComponent>(
                new MeshComponent(entity.attribute(SL_MESH_HANDLE), entity.firstChildElement().attribute(SL_MESH_ENTITY_MATERIAL_NAME), entity.attribute(SL_NAME))
                );

            QString cast_shadows = entity.attribute(SL_CAST_SHADOWS);
            if ( cast_shadows == SL_TRUE )
            {
                mesh->setCastShadows(true);
            }
            else if ( cast_shadows == SL_FALSE )
            {
                mesh->setCastShadows(false);
            }

            mesh->enable();
        }
        else if ( unknown_mesh.nodeName() == SL_MESH_PLANE )
        {
            const QDomElement& plane = unknown_mesh;
            if ( !plane.isNull() )
            {
                //create plane
                OgreProcedural::PlaneGenerator()
                    .setSizeX(plane.attribute(SL_MESH_PLANE_SIZEX).toFloat())
                    .setSizeY(plane.attribute(SL_MESH_PLANE_SIZEY).toFloat())
                    .setEnableNormals(plane.attribute(SL_MESH_PLANE_ENABLE_NORMALS).toInt())
                    .setNumSegX(plane.attribute(SL_MESH_PLANE_SEGMENTSX).toInt())
                    .setNumSegY(plane.attribute(SL_MESH_PLANE_SEGMENTSY).toInt())
                    .setNumTexCoordSet(plane.attribute(SL_MESH_PLANE_NUMTEXCOORD).toInt())
                    .setUTile(plane.attribute(SL_MESH_PLANE_UTILE).toFloat())
                    .setVTile(plane.attribute(SL_MESH_PLANE_VTILE).toFloat())
                    .setNormal(Ogre::Vector3( plane.firstChildElement(SL_MESH_PLANE_NORMAL).attribute(SL_X).toFloat(),
                    plane.firstChildElement(SL_MESH_PLANE_NORMAL).attribute(SL_Y).toFloat(),
                    plane.firstChildElement(SL_MESH_PLANE_NORMAL).attribute(SL_Z).toFloat()))
                    .realizeMesh(dt::Utils::toStdString(plane.attribute(SL_NAME)));

                //add mesh component
                auto mesh = node->addComponent<MeshComponent>(
                    new MeshComponent(plane.attribute(SL_NAME), plane.attribute(SL_MESH_PLANE_MATERIAL), plane.attribute(SL_NAME))
                    );

                mesh->enable();
            }
        }
    }

    return node;
}

Node::NodeSP SceneLoader::__loadLight(const QDomElement& og_component, Node::NodeSP dt_node)
{
    Node::NodeSP node = dt_node;

    if ( !og_component.isNull() )
    {
        QString name = og_component.attribute(SL_NAME);

        if ( node == nullptr )
        {
            node = mScene->addChildNode(new Node(name + "_node"));

            QDomElement pos = og_component.firstChildElement(SL_POS);
            QDomElement dir = og_component.firstChildElement(SL_LIGHT_DIRECTION);

            node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
                pos.attribute(SL_Z).toFloat());
            node->setDirection(Ogre::Vector3(dir.attribute(SL_X).toFloat(),
                dir.attribute(SL_Y).toFloat(), dir.attribute(SL_Z).toFloat()));
        }

        //add light component
        auto light = node->addComponent<LightComponent>(new LightComponent(name));
        auto og_light = light->getOgreLight();
        QDomElement colour_diffuse = og_component.firstChildElement(SL_LIGHT_DIFFUSE);
        QDomElement colour_specular = og_component.firstChildElement(SL_LIGHT_SPECULAR);
        QDomElement light_attenuation = og_component.firstChildElement(SL_LIGHT_ATTENUATION);

        //set light attributes
        og_light->setDiffuseColour(colour_diffuse.attribute(SL_COLOUR_R).toFloat(),
            colour_diffuse.attribute(SL_COLOUR_G).toFloat(),
            colour_diffuse.attribute(SL_COLOUR_B).toFloat());
        og_light->setSpecularColour(colour_specular.attribute(SL_COLOUR_R).toFloat(),
            colour_specular.attribute(SL_COLOUR_G).toFloat(),
            colour_specular.attribute(SL_COLOUR_B).toFloat());
        og_light->setAttenuation(light_attenuation.attribute(SL_LIGHT_ATTENUATION_RANGE).toFloat(),
            light_attenuation.attribute(SL_LIGHT_ATTENUATION_CONSTANT).toFloat(),
            light_attenuation.attribute(SL_LIGHT_ATTENUATION_LINEAR).toFloat(),
            light_attenuation.attribute(SL_LIGHT_ATTENUATION_QUADRATIC).toFloat());

        QString light_type = og_component.attribute(SL_LIGHT_TYPE);
        if ( light_type == SL_LIGHT_TYPE_POINT )
        {
            og_light->setType(Ogre::Light::LT_POINT);
        }
        else if ( light_type == SL_LIGHT_TYPE_DIRECTIONAL )
        {
            og_light->setType(Ogre::Light::LT_DIRECTIONAL);
        }
        else if ( light_type == SL_LIGHT_TYPE_SPOT )
        {
            og_light->setType(Ogre::Light::LT_SPOTLIGHT);

            QDomElement light_range = og_component.firstChildElement(SL_LIGHT_RANGE);

            og_light->setSpotlightRange(Ogre::Radian(light_range.attribute(SL_LIGHT_RANGE_INNER).toFloat()),
                Ogre::Radian(light_range.attribute(SL_LIGHT_RANGE_OUTER).toFloat()),
                light_range.attribute(SL_LIGHT_RANGE_FALLOFF).toFloat());
        }

        QString cast_shadows = og_component.attribute(SL_CAST_SHADOWS);
        if ( cast_shadows == SL_TRUE )
        {
            light->setCastShadows(true);
        }
        else if ( cast_shadows == SL_FALSE )
        {
            light->setCastShadows(false);
        }

        light->enable();
    }

    return node;
}

Node::NodeSP SceneLoader::__loadCamera(const QDomElement& og_component, Node::NodeSP dt_node)
{
    Node::NodeSP node = dt_node;

    if ( !og_component.isNull() )
    {
        QString name = og_component.attribute(SL_NAME);

        if ( node == nullptr )
        {
            node = mScene->addChildNode(new Node(name + "_node"));

            QDomElement pos = og_component.firstChildElement(SL_POS);
            QDomElement rot = og_component.firstChildElement(SL_ROT);

            node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
                pos.attribute(SL_Z).toFloat());
            node->setRotation(Ogre::Quaternion(rot.attribute(SL_QW).toFloat(),
                rot.attribute(SL_QX).toFloat(), rot.attribute(SL_QY).toFloat(), rot.attribute(SL_QZ).toFloat()));
        }

        //add camera component
        auto camera = node->addComponent<CameraComponent>(new CameraComponent(name));
        auto og_camera = camera->getCamera();
        QDomElement clipping = og_component.firstChildElement(SL_CAMERA_CLIPPING);

        //set camera attributes
        og_camera->setPolygonMode(Ogre::PolygonMode(og_component.attribute(SL_CAMERA_POLYGON_MODE).toInt()));
        og_camera->setFOVy(Ogre::Radian(og_component.attribute(SL_CAMERA_FOV).toFloat()));
        og_camera->setNearClipDistance(Ogre::Real(clipping.attribute(SL_CAMERA_CLIPPING_NEAR).toFloat()));
        og_camera->setFarClipDistance(Ogre::Real(clipping.attribute(SL_CAMERA_CLIPPING_FAR).toFloat()));

        camera->enable();
    }

    return node;
}

Node::NodeSP SceneLoader::__loadScriptPath(const QDomElement& og_component, Node::NodeSP dt_node)
{
    Node::NodeSP node = dt_node;

    if ( !og_component.isNull() )
    {
        QString name = og_component.attribute(SL_NAME);

        if ( node == nullptr )
        {
            node = mScene->addChildNode(new Node(name + "_node"));

            QDomElement pos = og_component.firstChildElement(SL_POS);

            node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
                pos.attribute(SL_Z).toFloat());
        }

        //add script path component
        auto script_path = node->addComponent<ScriptComponent>(new ScriptComponent(og_component.attribute(SL_SCPATH_PATH), name));

        //set script path attribute
        QString update_enable = og_component.attribute(SL_SCPATH_UPDATE_ENABLED);
        if ( update_enable == SL_TRUE )
        {
            script_path->SetUpdateEnabled(true);
        }
        else if ( update_enable == SL_FALSE )
        {
            script_path->SetUpdateEnabled(false);
        }

        QString enable = og_component.attribute(SL_COMPONENT_ENABLED);
        if ( enable == SL_TRUE )
        {
            script_path->enable();
        }
        else if ( enable == SL_FALSE )
        {
            script_path->disable();
        }
    }

    return node;
}

Node::NodeSP SceneLoader::__loadSound(const QDomElement& og_component, Node::NodeSP dt_node)
{
    Node::NodeSP node = dt_node;

    if ( !og_component.isNull() )
    {
        QString name = og_component.attribute(SL_NAME);

        if ( node == nullptr )
        {
            node = mScene->addChildNode(new Node(name + "_node"));

            QDomElement pos = og_component.firstChildElement(SL_POS);

            node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
                pos.attribute(SL_Z).toFloat());
        }

        //add sound component
        auto sound = node->addComponent<SoundComponent>(new SoundComponent(og_component.attribute(SL_SOUND_PATH), name));

        //set sound attributes
        sound->setSoundFileName(og_component.attribute(SL_SOUND_PATH));
        sound->setVolume(og_component.attribute(SL_SOUND_VOLUME).toFloat());
        /* loop attribute not used for now */

        QString enable = og_component.attribute(SL_COMPONENT_ENABLED);
        if ( enable == SL_TRUE )
        {
            sound->enable();
        }
        else if ( enable == SL_FALSE )
        {
            sound->disable();
        }
    }

    return node;
}

Node::NodeSP SceneLoader::__loadMusic(const QDomElement& og_component, Node::NodeSP dt_node)
{
    Node::NodeSP node = dt_node;

    if ( !og_component.isNull() )
    {
        QString name = og_component.attribute(SL_NAME);

        if ( node == nullptr )
        {
            node = mScene->addChildNode(new Node(name + "_node"));

            QDomElement pos = og_component.firstChildElement(SL_POS);

            node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
                pos.attribute(SL_Z).toFloat());
        }

        //add music component
        auto music = node->addComponent<MusicComponent>(new MusicComponent(og_component.attribute(SL_MUSIC_PATH), name));

        //set music attributes
        music->setVolume(og_component.attribute(SL_MUSIC_VOLUME).toFloat());
        /* loop attribute not used for now */

        QString enable = og_component.attribute(SL_COMPONENT_ENABLED);
        if ( enable == SL_TRUE )
        {
            music->enable();
        }
        else if ( enable == SL_FALSE )
        {
            music->disable();
        }
    }

    return node;
}

Node::NodeSP SceneLoader::__loadInteractor(const QDomElement& og_component, Node::NodeSP dt_node)
{
    Node::NodeSP node = dt_node;

    if ( !og_component.isNull() )
    {
        QString name = og_component.attribute(SL_NAME);

        if ( node == nullptr )
        {
            node = mScene->addChildNode(new Node(name + "_node"));

            QDomElement pos = og_component.firstChildElement(SL_POS);

            node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
                pos.attribute(SL_Z).toFloat());
        }

        //add interactor component
        QString type = og_component.attribute(SL_INTERACTOR_TYPE);
        std::tr1::shared_ptr<InteractionComponent> interactor;
        if ( type == SL_INTERACTOR_TYPE_RAYCASTING )
        {
            interactor = node->addComponent<InteractionComponent>(new RaycastComponent(name));
        }
        else if ( type == SL_INTERACTOR_TYPE_COLLISION )
        {
            interactor = node->addComponent<InteractionComponent>(new CollisionComponent(name));
        }

        //set interactor attributes
        interactor->setRange(og_component.attribute(SL_INTERACTOR_RANGE).toFloat());
        interactor->setOffset(og_component.attribute(SL_INTERACTOR_OFFSET).toFloat());
        interactor->setIntervalTime(og_component.attribute(SL_INTERACTOR_INTERVAL).toFloat());

        QString enable = og_component.attribute(SL_COMPONENT_ENABLED);
        if ( enable == SL_TRUE )
        {
            interactor->enable();
        }
        else if ( enable == SL_FALSE )
        {
            interactor->disable();
        }
    }

    return node;
}

Node::NodeSP SceneLoader::__loadPhysics(const QDomElement& og_component, Node::NodeSP dt_node)
{
    Node::NodeSP node = dt_node;
    if ( !og_component.isNull() )
    {
        QString name = og_component.attribute(SL_NAME);

        if ( node == nullptr )
        {
            node = mScene->addChildNode(new Node(name + "_node"));

            QDomElement pos = og_component.firstChildElement(SL_POS);

            node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
                pos.attribute(SL_Z).toFloat());
        }

        //get necessary attributes before initialize physics component
        QString shape = og_component.attribute(SL_PHYSICS_SHAPE);
        auto type = PhysicsBodyComponent::CONVEX;

        if ( shape == SL_PHYSICS_SHAPE_BOX )
        {
            type = PhysicsBodyComponent::BOX;
        }
        else if ( shape == SL_PHYSICS_SHAPE_CYLINDER )
        {
            type = PhysicsBodyComponent::CYLINDER;
        }
        else if ( shape == SL_PHYSICS_SHAPE_SPHERE )
        {
            type = PhysicsBodyComponent::SPHERE;
        }
        else if ( shape == SL_PHYSICS_SHAPE_TRIMESH )
        {
            type = PhysicsBodyComponent::TRIMESH;
        }

        //add physics component
        auto physics = node->addComponent<PhysicsBodyComponent>(
            new PhysicsBodyComponent(og_component.attribute(SL_PHYSICS_MESH_COM_NAME), name, type)
            );

        //set physics attributes
        QDomElement res_move = og_component.firstChildElement(SL_PHYSICS_RESMOVE);
        QDomElement res_rotate = og_component.firstChildElement(SL_PHYSICS_RESROTATE);
        QDomElement gravity = og_component.firstChildElement(SL_PHYSICS_GRAVITY);

        physics->setRestrictMovement(res_move.attribute(SL_X).toFloat(), res_move.attribute(SL_Y).toFloat(),
            res_move.attribute(SL_Z).toFloat());
        physics->setRestrictRotation(res_rotate.attribute(SL_X).toFloat(), res_move.attribute(SL_Y).toFloat(),
            res_move.attribute(SL_Z).toFloat());
        physics->setMass(og_component.attribute(SL_PHYSICS_MASS).toFloat());
        physics->setGravity(gravity.attribute(SL_X).toFloat(), gravity.attribute(SL_Y).toFloat(),
            gravity.attribute(SL_Z).toFloat());

        QString enable = og_component.attribute(SL_COMPONENT_ENABLED);
        if ( enable == SL_TRUE )
        {
            physics->enable();
        }
        else if ( enable == SL_FALSE )
        {
            physics->disable();
        }
    }

    return node;
}

Node::NodeSP SceneLoader::__loadController(const QDomElement& og_component, Node::NodeSP dt_node)
{
    Node::NodeSP node = dt_node;

    /*if ( !og_component.isNull() )
    {
    QString name = og_component.attribute(SL_NAME);

    if ( node == nullptr )
    {
    node = mScene->addChildNode(new Node(name + "_node"));

    QDomElement pos = og_component.firstChildElement(SL_POS);

    node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
    pos.attribute(SL_Z).toFloat());
    }

    //add controller component
    auto controller = node->addComponent<ControllerComponent>(new ControllerComponent(name));

    QString enable = og_component.attribute(SL_COMPONENT_ENABLED);
    if ( enable == SL_TRUE )
    {
    controller->enable();
    }
    else if ( enable == SL_FALSE )
    {
    controller->disable();
    }
    }*/

    return node;
}

Node::NodeSP SceneLoader::__loadTriggerArea(const QDomElement& og_component, Node::NodeSP dt_node)
{
    Node::NodeSP node = dt_node;

    if ( !og_component.isNull() )
    {
        QString name = og_component.attribute(SL_NAME);

        if ( node == nullptr )
        {
            node = mScene->addChildNode(new Node(name + "_node"));

            QDomElement pos = og_component.firstChildElement(SL_POS);

            node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
                pos.attribute(SL_Z).toFloat());
        }

        //get trigger area shape type
        btCollisionShape *pCS = nullptr;
        QDomElement scale = og_component.firstChildElement(SL_TRIGGER_SCALE);
        Ogre::Vector3 size(scale.attribute(SL_TRIGGER_SCALE_X).toFloat(),
            scale.attribute(SL_TRIGGER_SCALE_Y).toFloat(),
            scale.attribute(SL_TRIGGER_SCALE_Z).toFloat());

        auto trigger_shape = og_component.attribute(SL_TRIGGER_SHAPE).toInt();
        switch ( trigger_shape )
        {
        case BOX_SHAPE_PROXYTYPE :
            pCS = new btBoxShape(btVector3(1.0, 1.0, 1.0));
            break;
        case CYLINDER_SHAPE_PROXYTYPE :
            pCS = new btCylinderShape(btVector3(1.0, 1.0, 1.0));
            break;
        case SPHERE_SHAPE_PROXYTYPE :
            pCS = new btSphereShape(1.0);
            break;
        }
        pCS->setLocalScaling(BtOgre::Convert::toBullet(size));

        //add trigger area component
        auto trigger = node->addComponent<TriggerAreaComponent>(new TriggerAreaComponent(pCS, name));

        //set trigger area attributes
        QString enable = og_component.attribute(SL_COMPONENT_ENABLED);
        if ( enable == SL_TRUE )
        {
            trigger->enable();
        }
        else if ( enable == SL_FALSE )
        {
            trigger->disable();
        }
    }

    return node;
}

ParticleInfo load_ParticleInfo(const QDomElement &par_info) {
	ParticleInfo FireBack_num;
	auto TimeToLiveL = par_info.firstChildElement("TimeToLiveL");
	FireBack_num.TimeToLiveL = TimeToLiveL.text().toFloat();

	auto TimeToLiveR = par_info.firstChildElement("TimeToLiveR");
	FireBack_num.TimeToLiveR = TimeToLiveR.text().toFloat();

	//add by liujack
	auto ParticleCountLimit = par_info.firstChildElement("ParticleCountLimit");
	FireBack_num.ParticleCountLimit = ParticleCountLimit.text().toUInt();

	auto EmissionRate = par_info.firstChildElement("EmissionRate");
	FireBack_num.EmissionRate = EmissionRate.text().toUInt();

	auto DefaultDimensionsWidth = par_info.firstChildElement("DefaultDimensionsWidth");
	FireBack_num.DefaultDimensionsWidth = DefaultDimensionsWidth.text().toFloat();

	auto DefaultDimensionsHeight = par_info.firstChildElement("DefaultDimensionsHeight");
	FireBack_num.DefaultDimensionsHeight = DefaultDimensionsHeight.text().toFloat();

	auto EmitterColorStart_r = par_info.firstChildElement("EmitterColorStart_r");
	FireBack_num.EmitterColorStart.r = EmitterColorStart_r.text().toFloat();

	auto EmitterColorStart_g = par_info.firstChildElement("EmitterColorStart_g");
	FireBack_num.EmitterColorStart.g = EmitterColorStart_g.text().toFloat();

	auto EmitterColorStart_b = par_info.firstChildElement("EmitterColorStart_b");
	FireBack_num.EmitterColorStart.b = EmitterColorStart_b.text().toFloat();

	auto EmitterColorEnd_r = par_info.firstChildElement("EmitterColorEnd_r");
	FireBack_num.EmitterColorEnd.r = EmitterColorEnd_r.text().toFloat();

	auto EmitterColorEnd_g = par_info.firstChildElement("EmitterColorEnd_g");
	FireBack_num.EmitterColorEnd.g = EmitterColorEnd_g.text().toFloat();

	auto EmitterColorEnd_b = par_info.firstChildElement("EmitterColorEnd_b");
	FireBack_num.EmitterColorEnd.b = EmitterColorEnd_b.text().toFloat();
	//end

	auto time0 = par_info.firstChildElement("time0");
	FireBack_num.time0 = time0.text().toFloat();

	auto colour0r = par_info.firstChildElement("colour0r");
	FireBack_num.colour0.x = colour0r.text().toFloat();

	auto colour0g = par_info.firstChildElement("colour0g");
	FireBack_num.colour0.y = colour0g.text().toFloat();

	auto colour0b = par_info.firstChildElement("colour0b");
	FireBack_num.colour0.z = colour0b.text().toFloat();

	auto time1 = par_info.firstChildElement("time1");
	FireBack_num.time1 = time1.text().toFloat();

	auto colour1r = par_info.firstChildElement("colour1r");
	FireBack_num.colour1.x = colour1r.text().toFloat();

	auto colour1g = par_info.firstChildElement("colour1g");
	FireBack_num.colour1.y = colour1g.text().toFloat();

	auto colour1b = par_info.firstChildElement("colour1b");
	FireBack_num.colour1.z = colour1b.text().toFloat();

	auto time2 = par_info.firstChildElement("time2");
	FireBack_num.time2 = time2.text().toFloat();

	auto colour2r = par_info.firstChildElement("colour2r");
	FireBack_num.colour2.x = colour2r.text().toFloat();

	auto colour2g = par_info.firstChildElement("colour2g");
	FireBack_num.colour2.y = colour2g.text().toFloat();

	auto colour2b = par_info.firstChildElement("colour2b");
	FireBack_num.colour2.z = colour2b.text().toFloat();

	auto degree = par_info.firstChildElement("degree");
	FireBack_num.degree = degree.text().toFloat();

	auto MaterialHandle = par_info.firstChildElement("MaterialHandle");
	FireBack_num.MaterialHandle = MaterialHandle.text();

	return FireBack_num;
}

Node::NodeSP SceneLoader::__loadAlien(const QDomElement& og_node, Node::NodeSP dt_parent) 
{

    Node::NodeSP node = nullptr;
    if (!og_node.isNull())
    {
        QString node_name = og_node.attribute(SL_NAME);
        QString alien_name = og_node.attribute(SL_ALIEN_NAME);
        QString agent = og_node.attribute(SL_AGENT_TYPE);
        Alien *pAlien = new Alien(node_name, 
                                  alien_name + ".mesh",
                                  dt::PhysicsBodyComponent::BOX,
                                  100, 
                                  alien_name + "_walk",
                                  alien_name + "_jump",
                                  alien_name + "_run");
        pAlien->setMaxHealth(100);
        pAlien->setCurHealth(100);
        pAlien->setEyePosition(Ogre::Vector3(0, 1.8, -0.5));
		
        if (dt_parent)
            node = dt_parent->addChildNode(pAlien);
        else 
            node = mScene->addChildNode(pAlien);

        QDomElement pos = og_node.firstChildElement(SL_POS);
        QDomElement scale = og_node.firstChildElement(SL_SCALE);
		
        QDomElement rot = og_node.firstChildElement(SL_ORI);

        auto physics = pAlien->findComponent<dt::PhysicsBodyComponent>("physics_body");
        auto motion = physics->getRigidBody()->getMotionState();

        btTransform trans;
        motion->getWorldTransform(trans);

        trans.setOrigin(btVector3(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
            pos.attribute(SL_Z).toFloat()));
        trans.setRotation(btQuaternion(
            rot.attribute(SL_OX).toFloat(), rot.attribute(SL_OY).toFloat(), rot.attribute(SL_OZ).toFloat(),rot.attribute(SL_OW).toFloat()));

        motion->setWorldTransform(trans);
        physics->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(scale.attribute(SL_X).toFloat(), scale.attribute(SL_Y).toFloat(),
            scale.attribute(SL_Z).toFloat()));

        pAlien->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
            pos.attribute(SL_Z).toFloat());
        pAlien->setScale(Ogre::Vector3(scale.attribute(SL_X).toFloat(), scale.attribute(SL_Y).toFloat(),
            scale.attribute(SL_Z).toFloat()));

        if (agent == "HumanAgent")
        {
            pAlien->setEyePosition(Ogre::Vector3(0, 1.8, -1.0));
            HumanAgent* human_agent = new HumanAgent("Player");
            human_agent->attachTo(pAlien);
            EntityManager::get()->setHuman(pAlien);
        }
        if (agent == "AiAgent")
        {
            pAlien->setEyePosition(Ogre::Vector3(0, 5, 5));
            PlayerAIAgent *Ai_agent = new PlayerAIAgent("AiPlayer");
            Ai_agent->attachTo(pAlien);
            EntityManager::get()->addPlayer(pAlien);

        }

        QFile file("WeaponAttribute.xml");
        QDomDocument doc;

        if ( !file.open(QIODevice::ReadOnly) )
        {
            dt::Logger::get().error("Couldn't open file WeaponAttribute.xml");
            return nullptr;
        }
        if (!doc.setContent(&file))
            return nullptr;

        QDomElement root = doc.documentElement();

        QString weapon_id;

        QDomElement primary = og_node.firstChildElement(SL_ALIEN_PRIMARY);
        QDomElement secondary = og_node.firstChildElement(SL_ALIEN_SECONDARY);
        QDomElement throwable = og_node.firstChildElement(SL_ALIEN_THROWABLE);

        if (!primary.isNull()) {
            weapon_id = primary.attribute("content");
			
            QDomElement w_node = root.firstChildElement(weapon_id);

            auto type = w_node.firstChildElement("type");

            if (type.text() == "Primary") {
                uint16_t weapon_type = 0;

                auto power = w_node.firstChildElement("power");
                uint16_t power_num = power.text().toUInt();

                auto ammo_per_clip = w_node.firstChildElement("ammo_per_clip");
                uint16_t ammo_per_clip_num = ammo_per_clip.text().toUInt();		

                auto maximum_clip = w_node.firstChildElement("maximum_clip");
                uint16_t maximum_clip_num = maximum_clip.text().toUInt();

                auto is_one_shot = w_node.firstChildElement("is_one_shot");
                bool is_one_shot_num;
                if (is_one_shot.text() == "true")
                    is_one_shot_num = 1;
                else
                    is_one_shot_num = 0;

                auto interval = w_node.firstChildElement("interval");
                float interval_num = interval.text().toFloat();

                auto reload_time = w_node.firstChildElement("reload_time");
                float reload_time_num = reload_time.text().toFloat();

                auto range = w_node.firstChildElement("range");
                float range_num = range.text().toFloat();

                auto mass = w_node.firstChildElement("mass");
                float mass_num = mass.text().toFloat();

                ParticleInfo FireBack_num, Bomb_num;

				FireBack_num = load_ParticleInfo(w_node.firstChildElement("FireBack"));

				Bomb_num = load_ParticleInfo(w_node.firstChildElement("Bomb"));

				Weapon *pWeapon = new Weapon(weapon_id, 
					node_name,
					Weapon::WeaponType(weapon_type),
					power_num,
					maximum_clip_num,
					maximum_clip_num, 
					mass_num,
					ammo_per_clip_num,
					ammo_per_clip_num,
					is_one_shot_num,
					interval_num,
					reload_time_num,
					weapon_id + "_fire",
					weapon_id + "_reload_begin",
					weapon_id + "_reload_done",
					range_num,
					FireBack_num,
					Bomb_num);
				
				pAlien->addChildNode(pWeapon);
				
                pAlien->addWeapon(pWeapon);
            }
        }

        if (!secondary.isNull()) {
            weapon_id = secondary.attribute("content");;

            QDomElement w_node = root.firstChildElement(weapon_id);

            auto type = w_node.firstChildElement("type");

            if (type.text() == "Secondary") {
                uint16_t weapon_type = 1;

                auto power = w_node.firstChildElement("power");
                uint16_t power_num = power.text().toUInt();

                auto ammo_per_clip = w_node.firstChildElement("ammo_per_clip");
                uint16_t ammo_per_clip_num = ammo_per_clip.text().toUInt();		

                auto maximum_clip = w_node.firstChildElement("maximum_clip");
                uint16_t maximum_clip_num = maximum_clip.text().toUInt();

                auto is_one_shot = w_node.firstChildElement("is_one_shot");
                bool is_one_shot_num;
                if (is_one_shot.text() == "true")
                    is_one_shot_num = 1;
                else
                    is_one_shot_num = 0;

                auto interval = w_node.firstChildElement("interval");
                float interval_num = interval.text().toFloat();

                auto reload_time = w_node.firstChildElement("reload_time");
                float reload_time_num = reload_time.text().toFloat();

                auto range = w_node.firstChildElement("range");
                float range_num = range.text().toFloat();

                auto mass = w_node.firstChildElement("mass");
                float mass_num = mass.text().toFloat();

				ParticleInfo FireBack_num, Bomb_num;

				FireBack_num = load_ParticleInfo(w_node.firstChildElement("FireBack"));

				Bomb_num = load_ParticleInfo(w_node.firstChildElement("Bomb"));

				Weapon *pWeapon = new Weapon(weapon_id, 
					node_name,
					Weapon::WeaponType(weapon_type),
					power_num,
					maximum_clip_num,
					maximum_clip_num, 
					mass_num,
					ammo_per_clip_num,
					ammo_per_clip_num,
					is_one_shot_num,
					interval_num,
					reload_time_num,
					weapon_id + "_fire",
					weapon_id + "_reload_begin",
					weapon_id + "_reload_done",
					range_num,
					FireBack_num,
					Bomb_num);

                pAlien->addWeapon(pWeapon);
            }
        }

        if (!throwable.isNull()) {
            weapon_id = throwable.attribute("content");

            QDomElement w_node = root.firstChildElement(weapon_id);

            auto type = w_node.firstChildElement("type");

            if (type.text() == "Throwable") {
                uint16_t weapon_type = 1;

                auto power = w_node.firstChildElement("power");
                uint16_t power_num = power.text().toUInt();

                auto ammo_per_clip = w_node.firstChildElement("ammo_per_clip");
                uint16_t ammo_per_clip_num = ammo_per_clip.text().toUInt();		

                auto maximum_clip = w_node.firstChildElement("maximum_clip");
                uint16_t maximum_clip_num = maximum_clip.text().toUInt();

                auto is_one_shot = w_node.firstChildElement("is_one_shot");
                bool is_one_shot_num;
                if (is_one_shot.text() == "true")
                    is_one_shot_num = 1;
                else
                    is_one_shot_num = 0;

                auto interval = w_node.firstChildElement("interval");
                float interval_num = interval.text().toFloat();

                auto reload_time = w_node.firstChildElement("reload_time");
                float reload_time_num = reload_time.text().toFloat();

                auto range = w_node.firstChildElement("range");
                float range_num = range.text().toFloat();

                auto mass = w_node.firstChildElement("mass");
                float mass_num = mass.text().toFloat();

                ParticleInfo FireBack_num, Bomb_num;

				FireBack_num = load_ParticleInfo(w_node.firstChildElement("FireBack"));

				Bomb_num = load_ParticleInfo(w_node.firstChildElement("Bomb"));

                Weapon *pWeapon = new Weapon(weapon_id, 
                                     node_name,
                                     Weapon::WeaponType(weapon_type),
                                     power_num,
                                     maximum_clip_num,
                                     maximum_clip_num, 
                                     mass_num,
                                     ammo_per_clip_num,
                                     ammo_per_clip_num,
                                     is_one_shot_num,
                                     interval_num,
									 reload_time_num,
                                     weapon_id + "_fire",
                                     weapon_id + "_reload_begin",
                                     weapon_id + "_reload_done",
                                     range_num,
									 FireBack_num,
									 Bomb_num);

                pAlien->addWeapon(pWeapon);
            }
        }
    }

    return node;
}


Node::NodeSP SceneLoader::__loadAmmo(const QDomElement& og_node, Node::NodeSP dt_parent) 
{
    Node::NodeSP node = nullptr;
    if (!og_node.isNull())
    {
        QString node_name = og_node.attribute(SL_NAME);
        QString ammo_name = og_node.attribute(SL_AMMO_NAME);
        QString num_clip = og_node.attribute(SL_AMMO_NUM_CLIP);
        QString weapon_type = og_node.attribute(SL_AMMO_TYPE);

        Ammo *pAmmo = new Ammo(ammo_name, 
            node_name,
            num_clip.toInt(),
            Weapon::WeaponType(weapon_type.toInt()));

        if (dt_parent)
            node = dt_parent->addChildNode(pAmmo);
        else 
            node = mScene->addChildNode(pAmmo);
        /*
        QDomElement pos = og_node.firstChildElement(SL_POS);
        QDomElement scale = og_node.firstChildElement(SL_SCALE);
        QDomElement rot = og_node.firstChildElement(SL_ORI);


        auto physics = pAmmo->findComponent<dt::PhysicsBodyComponent>("physics_body");
        auto motion = physics->getRigidBody()->getMotionState();
        btTransform trans;
        motion->getWorldTransform(trans);

        trans.setOrigin(btVector3(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
        pos.attribute(SL_Z).toFloat()));
        trans.setRotation(btQuaternion(
        rot.attribute(SL_OX).toFloat(), rot.attribute(SL_OY).toFloat(), rot.attribute(SL_OZ).toFloat(),rot.attribute(SL_OW).toFloat()));

        motion->setWorldTransform(trans);
        physics->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(scale.attribute(SL_X).toFloat(), scale.attribute(SL_Y).toFloat(),
        scale.attribute(SL_Z).toFloat()));

        pAmmo->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
        pos.attribute(SL_Z).toFloat());
        pAmmo->setScale(Ogre::Vector3(scale.attribute(SL_X).toFloat(), scale.attribute(SL_Y).toFloat(),
        scale.attribute(SL_Z).toFloat()));*/

        QDomElement pos = og_node.firstChildElement(SL_POS);
        QDomElement scale = og_node.firstChildElement(SL_SCALE);
        QDomElement rot = og_node.firstChildElement(SL_ORI);
        node->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
        node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
            pos.attribute(SL_Z).toFloat());
        node->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();
        /*	node->setRotation(Ogre::Quaternion(rot.attribute(SL_OW).toFloat(),
        rot.attribute(SL_OX).toFloat(), rot.attribute(SL_OY).toFloat(), rot.attribute(SL_OZ).toFloat()));
        node->setScale(Ogre::Vector3(scale.attribute(SL_X).toFloat(), scale.attribute(SL_Y).toFloat(),
        scale.attribute(SL_Z).toFloat()));		*/
        //node->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
        //node->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();	

    }
    return node;
}

Node::NodeSP SceneLoader::__loadCrystal(const QDomElement& og_node, Node::NodeSP dt_parent) 
{
    Node::NodeSP node = nullptr;
    if (!og_node.isNull())
    {
        QString node_name = og_node.attribute(SL_NAME);
        QString crystal_name = og_node.attribute(SL_CRYSTAL_NAME);
        QString unlock_time = og_node.attribute(SL_CRYSTAL_UNLOCKTIME);
        Crystal *pCrystal = new Crystal(crystal_name, 
            node_name,     
            unlock_time.toDouble());
        if (dt_parent)
            node = dt_parent->addChildNode(pCrystal);
        else  
            node = mScene->addChildNode(pCrystal);
        /*
        QDomElement pos = og_node.firstChildElement(SL_POS);
        QDomElement scale = og_node.firstChildElement(SL_SCALE);
        QDomElement rot = og_node.firstChildElement(SL_ORI);


        auto physics = pCrystal->findComponent<dt::PhysicsBodyComponent>("physics_body");
        auto motion = physics->getRigidBody()->getMotionState();
        btTransform trans;
        motion->getWorldTransform(trans);

        trans.setOrigin(btVector3(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
        pos.attribute(SL_Z).toFloat()));
        trans.setRotation(btQuaternion(
        rot.attribute(SL_OX).toFloat(), rot.attribute(SL_OY).toFloat(), rot.attribute(SL_OZ).toFloat(),rot.attribute(SL_OW).toFloat()));

        motion->setWorldTransform(trans);
        physics->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(scale.attribute(SL_X).toFloat(), scale.attribute(SL_Y).toFloat(),
        scale.attribute(SL_Z).toFloat()));

        pCrystal->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
        pos.attribute(SL_Z).toFloat());
        pCrystal->setScale(Ogre::Vector3(scale.attribute(SL_X).toFloat(), scale.attribute(SL_Y).toFloat(),
        scale.attribute(SL_Z).toFloat()));*/

        QDomElement pos = og_node.firstChildElement(SL_POS);
        QDomElement scale = og_node.firstChildElement(SL_SCALE);
        QDomElement rot = og_node.firstChildElement(SL_ORI);
        node->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
        node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
            pos.attribute(SL_Z).toFloat());
        node->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();
        node->setRotation(Ogre::Quaternion(rot.attribute(SL_OW).toFloat(),
            rot.attribute(SL_OX).toFloat(), rot.attribute(SL_OY).toFloat(), rot.attribute(SL_OZ).toFloat()));
        node->setScale(Ogre::Vector3(scale.attribute(SL_X).toFloat(), scale.attribute(SL_Y).toFloat(),
            scale.attribute(SL_Z).toFloat()));	
        node->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
        node->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();	


    }
    return node;
}

Node::NodeSP SceneLoader::__loadFirstAidKit(const QDomElement& og_node, Node::NodeSP dt_parent) 
{
    Node::NodeSP node = nullptr;
    if (!og_node.isNull())
    {
        QString node_name = og_node.attribute(SL_NAME);
        QString first_aid_kit_name = og_node.attribute(SL_FIRSTAIDKIT_NAME);
        QString recovery_val_time = og_node.attribute(SL_RECOVERYVAL);
        FirstAidKit *pFirstAidKit = new FirstAidKit(first_aid_kit_name, 
            node_name,
            recovery_val_time.toInt());
        if (dt_parent)
            node = dt_parent->addChildNode(pFirstAidKit);
        else  
            node = mScene->addChildNode(pFirstAidKit);
        /*
        QDomElement pos = og_node.firstChildElement(SL_POS);
        QDomElement scale = og_node.firstChildElement(SL_SCALE);
        QDomElement rot = og_node.firstChildElement(SL_ORI);

        auto physics = pFirstAidKit->findComponent<dt::PhysicsBodyComponent>("physics_body");
        auto motion = physics->getRigidBody()->getMotionState();
        btTransform trans;
        motion->getWorldTransform(trans);

        trans.setOrigin(btVector3(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
        pos.attribute(SL_Z).toFloat()));
        trans.setRotation(btQuaternion(
        rot.attribute(SL_OX).toFloat(), rot.attribute(SL_OY).toFloat(), rot.attribute(SL_OZ).toFloat(),rot.attribute(SL_OW).toFloat()));

        motion->setWorldTransform(trans);
        physics->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(scale.attribute(SL_X).toFloat(), scale.attribute(SL_Y).toFloat(),
        scale.attribute(SL_Z).toFloat()));

        pFirstAidKit->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
        pos.attribute(SL_Z).toFloat());
        pFirstAidKit->setScale(Ogre::Vector3(scale.attribute(SL_X).toFloat(), scale.attribute(SL_Y).toFloat(),
        scale.attribute(SL_Z).toFloat()));*/
        QDomElement pos = og_node.firstChildElement(SL_POS);
        QDomElement scale = og_node.firstChildElement(SL_SCALE);
        QDomElement rot = og_node.firstChildElement(SL_ORI);
        node->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
        node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
            pos.attribute(SL_Z).toFloat());
        node->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();
        node->setRotation(Ogre::Quaternion(rot.attribute(SL_OW).toFloat(),
            rot.attribute(SL_OX).toFloat(), rot.attribute(SL_OY).toFloat(), rot.attribute(SL_OZ).toFloat()));
        node->setScale(Ogre::Vector3(scale.attribute(SL_X).toFloat(), scale.attribute(SL_Y).toFloat(),
            scale.attribute(SL_Z).toFloat()));		
        node->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
        node->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();	
    }
    return node;
}

Node::NodeSP SceneLoader::__loadMonster(const QDomElement& og_node, Node::NodeSP dt_parent) 
{
    Node::NodeSP node = nullptr;
    if (!og_node.isNull())
    {
        QString node_name = og_node.attribute(SL_NAME);
        QString monster_id = og_node.attribute(SL_MONSTER_ID);

        QFile file("MonsterAttribute.xml");
        QDomDocument doc;
        if ( !file.open(QIODevice::ReadOnly) )
        {
            dt::Logger::get().error("Couldn't open file MonsterAttribute.xml");
            return nullptr;
        }
        if (!doc.setContent(&file))
            return nullptr;

        QDomElement root = doc.documentElement();
        QDomElement w_node = root.firstChildElement(monster_id);

        auto mass = w_node.firstChildElement("mass");
        uint16_t mass_num = mass.text().toUInt();

        auto power = w_node.firstChildElement("power");
        uint16_t power_num = power.text().toUInt();

        auto interval = w_node.firstChildElement("interval");
        float interval_num = interval.text().toFloat();

        auto range = w_node.firstChildElement("range");
        float range_num = range.text().toFloat();

        Monster *pMonster = new Monster(node_name, 
            monster_id + ".mesh",
            dt::PhysicsBodyComponent::BOX,
            1,
            monster_id + "_walk",
            monster_id + "_jump",
            monster_id + "_run",
            monster_id + "_attack",
            power_num,
            range_num,
            interval_num);

        pMonster->setMaxHealth(100);
        pMonster->setCurHealth(100);

        if (dt_parent)
            node = dt_parent->addChildNode(pMonster);
        else  
            node = mScene->addChildNode(pMonster);

        EntityManager::get()->addMonster(pMonster);

        QDomElement pos = og_node.firstChildElement(SL_POS);
        QDomElement scale = og_node.firstChildElement(SL_SCALE);
        QDomElement rot = og_node.firstChildElement(SL_ORI);

        auto physics = pMonster->findComponent<dt::PhysicsBodyComponent>("physics_body");
        auto motion = physics->getRigidBody()->getMotionState();
        btTransform trans;
        motion->getWorldTransform(trans);

        trans.setOrigin(btVector3(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
            pos.attribute(SL_Z).toFloat()));
        trans.setRotation(btQuaternion(
            rot.attribute(SL_OX).toFloat(), rot.attribute(SL_OY).toFloat(), rot.attribute(SL_OZ).toFloat(),rot.attribute(SL_OW).toFloat()));

        motion->setWorldTransform(trans);
        physics->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(scale.attribute(SL_X).toFloat(), scale.attribute(SL_Y).toFloat(),
            scale.attribute(SL_Z).toFloat()));

        pMonster->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
            pos.attribute(SL_Z).toFloat());
        pMonster->setScale(Ogre::Vector3(scale.attribute(SL_X).toFloat(), scale.attribute(SL_Y).toFloat(),
            scale.attribute(SL_Z).toFloat()));

        MonsterAIAgent *agent = new MonsterAIAgent(monster_id + "_agent");
        agent->attachTo(pMonster);
    }
    return node;
}


Node::NodeSP SceneLoader::__loadWeapon(const QDomElement& og_node, Node::NodeSP dt_parent) 
{
    Node::NodeSP node = nullptr;
    if (!og_node.isNull())
    {
        QString node_name = og_node.attribute(SL_NAME);
        QString weapon_id = og_node.attribute(SL_WEAPON_ID);
        QFile file("WeaponAttribute.xml");
        QDomDocument doc;
        if ( !file.open(QIODevice::ReadOnly) )
        {
            dt::Logger::get().error("Couldn't open file WeaponAttribute.xml");
            return nullptr;
        }

        if (!doc.setContent(&file))
            return nullptr;
		
        QDomElement root = doc.documentElement();
        QDomElement w_node = root.firstChildElement(weapon_id);
		
        auto type = w_node.firstChildElement("type");
        uint16_t weapon_type;
        if (type.text() == "Primary")
            weapon_type = 0;
        else if (type.text() == "Secondery")
            weapon_type = 1;
        else
            weapon_type = 2;

        auto power = w_node.firstChildElement("power");
        uint16_t power_num = power.text().toUInt();
		
        auto ammo_per_clip = w_node.firstChildElement("ammo_per_clip");
        uint16_t ammo_per_clip_num = ammo_per_clip.text().toUInt();		
		
        auto maximum_clip = w_node.firstChildElement("maximum_clip");
        uint16_t maximum_clip_num = maximum_clip.text().toUInt();

        auto is_one_shot = w_node.firstChildElement("is_one_shot");
        bool is_one_shot_num;
        if (is_one_shot.text() == "true")
            is_one_shot_num = 1;
        else
            is_one_shot_num = 0;

        auto interval = w_node.firstChildElement("interval");
        float interval_num = interval.text().toFloat();

        auto reload_time = w_node.firstChildElement("reload_time");
        float reload_time_num = reload_time.text().toFloat();

        auto range = w_node.firstChildElement("range");
        float range_num = range.text().toFloat();

        auto mass = w_node.firstChildElement("mass");
        float mass_num = mass.text().toFloat();

        ParticleInfo FireBack_num, Bomb_num;

		FireBack_num = load_ParticleInfo(w_node.firstChildElement("FireBack"));

		Bomb_num = load_ParticleInfo(w_node.firstChildElement("Bomb"));

        Weapon *pWeapon = new Weapon(weapon_id, 
                                     node_name,
                                     Weapon::WeaponType(weapon_type),
                                     power_num,
                                     maximum_clip_num,
                                     maximum_clip_num, 
                                     mass_num,
                                     ammo_per_clip_num,
                                     ammo_per_clip_num,
                                     is_one_shot_num,
                                     interval_num,
									 reload_time_num,
                                     weapon_id + "_fire",
                                     weapon_id + "_reload_begin",
                                     weapon_id + "_reload_done",
                                     range_num,
									 FireBack_num,
									 Bomb_num);
										  								
        if (dt_parent)
            node = dt_parent->addChildNode(pWeapon);
        else  
            node = mScene->addChildNode(pWeapon);

        QDomElement pos = og_node.firstChildElement(SL_POS);
        QDomElement scale = og_node.firstChildElement(SL_SCALE);
        QDomElement rot = og_node.firstChildElement(SL_ORI);
        node->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
        node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
            pos.attribute(SL_Z).toFloat());
        node->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();
        node->setRotation(Ogre::Quaternion(rot.attribute(SL_OW).toFloat(),
            rot.attribute(SL_OX).toFloat(), rot.attribute(SL_OY).toFloat(), rot.attribute(SL_OZ).toFloat()));

        node->setScale(Ogre::Vector3(scale.attribute(SL_X).toFloat(), scale.attribute(SL_Y).toFloat(),
            scale.attribute(SL_Z).toFloat()));	
        node->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
        node->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();
    }
    return node;
}


FlameInfo load_FlameInfo(const QDomElement& frame_info) {
	FlameInfo ans;

	auto mName = frame_info.firstChildElement("mName");
	ans.mName = mName.text();

	auto mPosition = frame_info.firstChildElement("mPosition");
		auto x = mPosition.firstChildElement("x");
		float x_num = x.text().toFloat();

		auto y = mPosition.firstChildElement("y");
		float y_num = y.text().toFloat();

		auto z = mPosition.firstChildElement("z");
		float z_num = z.text().toFloat();
	ans.mPosition = Ogre::Vector3(x_num, y_num, z_num);

	auto mDirection = frame_info.firstChildElement("mDirection");
		x = mDirection.firstChildElement("x");
		x_num = x.text().toFloat();

		y = mDirection.firstChildElement("y");
		y_num = y.text().toFloat();

		z = mDirection.firstChildElement("z");
		z_num = z.text().toFloat();
	ans.mDirection = Ogre::Vector3(x_num, y_num, z_num);

	auto mParticle = frame_info.firstChildElement("mParticle");
	ans.mParticle = load_ParticleInfo(mParticle);
	return ans;
}

Node::NodeSP SceneLoader::__loadSpaceship(const QDomElement& og_node, Node::NodeSP dt_parent) 
{
    Node::NodeSP node = nullptr;
    if (!og_node.isNull())
    {
        QString node_name = og_node.attribute(SL_NAME);
        QString Spaceship_ID = og_node.attribute(SL_SPACESHIP_ID);
       
	    QFile file("SpaceshipAttribute.xml");
        QDomDocument doc;
        if ( !file.open(QIODevice::ReadOnly) )
        {
            dt::Logger::get().error("Couldn't open file SpaceshipAttribute.xml");
            return nullptr;
        }

        if (!doc.setContent(&file))
            return nullptr;
		
        QDomElement root = doc.documentElement();
        QDomElement w_node = root.firstChildElement(Spaceship_ID);
		

		auto mesh_handle = w_node.firstChildElement("mesh_handle").text();

		auto mass = w_node.firstChildElement("mass");
		float mass_num = mass.text().toFloat();

		auto attack_value = w_node.firstChildElement("attack_value");
		uint16_t attack_value_num = attack_value.text().toInt();

		auto attack_range = w_node.firstChildElement("attack_range");
		float attack_range_num = attack_range.text().toFloat();

		auto attack_interval = w_node.firstChildElement("attack_interval");
		float attack_interval_num = attack_interval.text().toFloat();

		auto attack_sound_handle = w_node.firstChildElement("attack_sound_handle").text();

		auto flying_sound_handle = w_node.firstChildElement("flying_sound_handle").text();

		auto rise_sound_handle = w_node.firstChildElement("rise_sound_handle").text();

		auto fall_sound_handle = w_node.firstChildElement("fall_sound_handle").text();

		auto max_speed = w_node.firstChildElement("max_speed");
		float max_speed_num = max_speed.text().toFloat();

		auto speed_per_frame = w_node.firstChildElement("speed_per_frame");
		float speed_per_frame_num = speed_per_frame.text().toFloat();

		auto max_lean_angle = w_node.firstChildElement("max_lean_angle");
		float max_lean_angle_num = max_lean_angle.text().toFloat();

		auto angle_per_frame = w_node.firstChildElement("angle_per_frame");
		float angle_per_frame_num = angle_per_frame.text().toFloat();

		auto parallel_move_speed = w_node.firstChildElement("parallel_move_speed");
		float parallel_move_speed_num = parallel_move_speed.text().toFloat();

		auto up_down_speed = w_node.firstChildElement("up_down_speed");
		float up_down_speed_num = up_down_speed.text().toFloat();

		auto ammo_fire_back = w_node.firstChildElement("ammo_fire_back");
		ParticleInfo ammo_fire_back_num = load_ParticleInfo(ammo_fire_back);

		auto ammo_bomb = w_node.firstChildElement("ammo_bomb");
		ParticleInfo ammo_bomb_num = load_ParticleInfo(ammo_bomb);

		auto flame_effect0 = w_node.firstChildElement("flame_effect0");
		auto flame_effect0_num = load_FlameInfo(flame_effect0);

		auto flame_effect1 = w_node.firstChildElement("flame_effect1");
		auto flame_effect1_num = load_FlameInfo(flame_effect1); 

		auto bullet_handle = w_node.firstChildElement("bullet_handle").text();

		std::vector<FlameInfo> temp;

		temp.push_back(flame_effect0_num);
		temp.push_back(flame_effect1_num);

		Spaceship *pSpaceship = new Spaceship(node_name, 
			mesh_handle, 
			dt::PhysicsBodyComponent::BOX, 
			mass_num,
			attack_value_num,
			attack_range_num,
			attack_interval_num,
			attack_sound_handle,
			flying_sound_handle,
			rise_sound_handle,
			fall_sound_handle,
			max_speed_num,
			speed_per_frame_num,
			max_lean_angle_num,
			angle_per_frame_num,
			parallel_move_speed_num, 
			up_down_speed_num,
		    ammo_fire_back_num,
			ammo_bomb_num,
			temp,
			bullet_handle);
        //256.0f,
        //256.0f / 8192,
        //32.0f,
        //16.0f);
        if (dt_parent)
            node = dt_parent->addChildNode(pSpaceship);
        else  
            node = mScene->addChildNode(pSpaceship);
        /*
        QDomElement pos = og_node.firstChildElement(SL_POS);
        QDomElement scale = og_node.firstChildElement(SL_SCALE);
        QDomElement rot = og_node.firstChildElement(SL_ORI);

        auto physics = pSpaceship->findComponent<dt::PhysicsBodyComponent>("physics_body");
        auto motion = physics->getRigidBody()->getMotionState();
        btTransform trans;
        motion->getWorldTransform(trans);

        trans.setOrigin(btVector3(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
        pos.attribute(SL_Z).toFloat()));
        trans.setRotation(btQuaternion(
        rot.attribute(SL_OX).toFloat(), rot.attribute(SL_OY).toFloat(), rot.attribute(SL_OZ).toFloat(),rot.attribute(SL_OW).toFloat()));

        motion->setWorldTransform(trans);
        physics->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(scale.attribute(SL_X).toFloat(), scale.attribute(SL_Y).toFloat(),
        scale.attribute(SL_Z).toFloat()));

        pSpaceship->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
        pos.attribute(SL_Z).toFloat());
        pSpaceship->setScale(Ogre::Vector3(scale.attribute(SL_X).toFloat(), scale.attribute(SL_Y).toFloat(),
        scale.attribute(SL_Z).toFloat()));
        */
        QDomElement pos = og_node.firstChildElement(SL_POS);
        QDomElement scale = og_node.firstChildElement(SL_SCALE);
        QDomElement rot = og_node.firstChildElement(SL_ORI);
        node->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
        node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
            pos.attribute(SL_Z).toFloat());
        node->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();
        node->setRotation(Ogre::Quaternion(rot.attribute(SL_OW).toFloat(),
            rot.attribute(SL_OX).toFloat(), rot.attribute(SL_OY).toFloat(), rot.attribute(SL_OZ).toFloat()));
        node->setScale(Ogre::Vector3(scale.attribute(SL_X).toFloat(), scale.attribute(SL_Y).toFloat(),
            scale.attribute(SL_Z).toFloat()));	
        node->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
        node->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();		
    }
    return node;
}

Node::NodeSP SceneLoader::__loadPoint(const QDomElement& og_node, Node::NodeSP dt_parent) 
{
    Node::NodeSP node = nullptr;
    if (!og_node.isNull())
    {
        QDomElement pos = og_node.firstChildElement(SL_POS);
        QString id = og_node.attribute(SL_POINT_ID);
        AIDivideAreaManager::get()->addArea(Ogre::Vector3(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(), pos.attribute(SL_Z).toFloat()), id.toInt());
    }
    return node;
}

Node::NodeSP SceneLoader::__loadEdge(const QDomElement& og_node, Node::NodeSP dt_parent) 
{
    Node::NodeSP node = nullptr;
    if (!og_node.isNull())
    {
        QString Lp = og_node.attribute(SL_EDGE_LP);
        QString Rp = og_node.attribute(SL_EDGE_RP);
        AIDivideAreaManager::get()->addEdge(Lp.toInt(), Rp.toInt());
    }
    return node;
}