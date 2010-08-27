/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
LGPL like the rest of the engine.
-----------------------------------------------------------------------------
*/
/*
-----------------------------------------------------------------------------
Filename:    ExampleApplication.h
Description: Base class for all the OGRE examples
-----------------------------------------------------------------------------
*/

#ifndef __ExampleApplication_H__
#define __ExampleApplication_H__



#include <QOgreWidget.h>

#include <ExampleFrameListener.h>

#include <Ogre.h>
#include <OgreConfigFile.h>

using namespace Ogre;

/** Base class which manages the standard startup of an Ogre application.
    Designed to be subclassed for specific examples if required.
*/
class ExampleApplication : public QOgreWidget
{
public:
    /// Standard constructor
    ExampleApplication (QWidget *parent = 0, Qt::WFlags f = 0) :
        QOgreWidget (parent, f)
    {
        mFrameListener = 0;
        isInit = false;
    }
    /// Standard destructor
    virtual ~ExampleApplication()
    {
        if (mFrameListener)
            delete mFrameListener;
    }

    /// Override the show  method to initialize our stuff.
    /// we should only do this once.
    virtual void show(void)
    {
        if (isInit) return;
        isInit = true;

        setupResources();
        chooseSceneManager();
        createCamera();
        createViewports();

        // Set default mipmap level (NB some APIs ignore this)
        TextureManager::getSingleton().setDefaultNumMipmaps(5);

        // Create any resource listeners (for loading screens)
        createResourceListener();
        // Load resources
        loadResources();

        // Create the scene
        createScene();

        createFrameListener();
        std::cout << __FILE__ << ":" << __LINE__ << std::endl;
    }

protected:
    Camera* mCamera;
    Viewport *m_vp;
    SceneManager* mSceneMgr;
    ExampleFrameListener* mFrameListener;
    bool isInit;

    void resizeEvent (QResizeEvent *event) {
        if (myRenderWindow) {
            myRenderWindow->resize (width(), height());
            myRenderWindow->windowMovedOrResized();
        }
        if (mCamera)
            mCamera->setAspectRatio(Ogre::Real(m_vp->getActualWidth()) / Ogre::Real(m_vp->getActualHeight()));
    }


    // These internal methods package up the stages in the startup process
    virtual void chooseSceneManager(void)
    { // Create the SceneManager, in this case a generic one
        mSceneMgr = theOgreRoot->createSceneManager(ST_GENERIC, "ExampleSMInstance" + Ogre::StringConverter::toString ((unsigned long)this));
    }
    virtual void createCamera(void)
    {
        // Create the camera
        mCamera = mSceneMgr->createCamera("PlayerCam" + Ogre::StringConverter::toString ((unsigned long)this));

        // Position it at 500 in Z direction
        mCamera->setPosition(Vector3(0,0,500));
        // Look back along -Z
        mCamera->lookAt(Vector3(0,0,-300));
        mCamera->setNearClipDistance(5);

    }
    virtual void createFrameListener(void)
    {
        mFrameListener= new ExampleFrameListener(myRenderWindow, mCamera);
        mFrameListener->showDebugOverlay(true);
        theOgreRoot->addFrameListener(mFrameListener);
    }

    virtual void createScene(void) = 0;    // pure virtual - this has to be overridden

    virtual void destroyScene(void){}    // Optional to override this

    virtual void createViewports(void)
    {
        // Create one viewport, entire window
        m_vp = myRenderWindow->addViewport(mCamera);
        m_vp->setBackgroundColour(ColourValue(0,0,0));

        // Alter the camera aspect ratio to match the viewport
        mCamera->setAspectRatio(
            Real(m_vp->getActualWidth()) / Real(m_vp->getActualHeight()));
    }

    /// Method which will define the source of resources (other than current folder)
    virtual void setupResources(void)
    {
        std::cout << __FILE__ << ":" << __LINE__ << std::endl;
        // Load resource paths from config file
        ConfigFile cf;
        cf.load("resources.cfg");

        // Go through all sections & settings in the file
        ConfigFile::SectionIterator seci = cf.getSectionIterator();

        String secName, typeName, archName;
        while (seci.hasMoreElements())
        {
            secName = seci.peekNextKey();
            ConfigFile::SettingsMultiMap *settings = seci.getNext();
            ConfigFile::SettingsMultiMap::iterator i;
            for (i = settings->begin(); i != settings->end(); ++i)
            {
                typeName = i->first;
                archName = i->second;
                ResourceGroupManager::getSingleton().addResourceLocation(
                    archName, typeName, secName);
            }
        }
    }

	/// Optional override method where you can create resource listeners (e.g. for loading screens)
	virtual void createResourceListener(void)
	{

	}

	/// Optional override method where you can perform resource group loading
	/// Must at least do ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	virtual void loadResources(void)
	{
		// Initialise, parse scripts etc
		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	}



};


#endif
/* vim:et:sts=4:ts=4:sw=4
 */

