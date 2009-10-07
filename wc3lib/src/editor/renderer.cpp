#include <Ogre.h>	
#include <OgreErrorDialog.h>

#include "renderer.hpp"

namespace wc3lib
{

namespace editor
{

void Renderer::start()
{
	try
	{
		this->m_root = new Root("../../plugins.cfg", "../ogre.cfg", "ogre.log");
		
		if (!this->m_root->restoreConfig())
		{
			if (!this->m_root->showConfigDialog())
			{
				return;
			}
		}
		this->m_renderWindow = this->m_root->initialise(true, "Renderer Application");
		
		this->m_sceneManager = this->m_root->createSceneManager(ST_GENERIC, "sceneManager");
		// create main camera (for editor)
		this->m_camera = this->m_sceneManager->createCamera("Camera");
		this->m_camera->setPosition(Ogre::Vector3(0.0f, 0.0f, 500.0f));
		this->m_camera->lookAt(Ogre::Vector3(0.0f, 0.0f, 0.0f));
		this->m_camera->setNearClipDistance(5.0f);
		this->m_camera->setFarClipDistance(5000.0f);
		
		this->m_viewport = this->m_renderWindow->addViewport(this->m_camera);
		this->m_viewport->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f));
		this->m_cCamera->setAspectRatio(Ogre::Real(this->m_viewport->getActualWidth()) / Ogre::Real(this->m_viewport->getActualHeight()));
		
		this->m_root->startRendering();

	}
	catch (Exception &exception)
	{
		Ogre::ErrorDialog *dialog = Ogre::PlatformManager::getSingleton().createErrorDialog();
		dialog->display(exception.getFullDescription());
		delete dlg;	
	}
}

}

}
