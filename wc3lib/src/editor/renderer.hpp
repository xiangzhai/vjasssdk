#ifndef WC3LIB_RENDERER_HPP
#define WC3LIB_RENDERER_HPP

namespace Ogre
{

class Root;
class RenderWindow;
class SceneManager;
class Camera;
class Viewport;
	
}

namespace wc3lib
{

namespace editor
{

/**
* Provides simple Warcraft 3 editor-like render window and manager. Is able to render MDX, MDL and BLP files.
* @todo Add methods like addMdx(<coordinates>), addMdl(<coordinates>).
*/
class Renderer
{
	public:
		void start();
	
	private:
		class Ogre::Root *m_root;
		class Ogre::RenderWindow *m_renderWindow;
		class Ogre::SceneManager *m_sceneManager; //class Ogre::DefaultSceneManager *m_defaultSceneManager;
		class Ogre::Camera *m_camera; // main camera
		class Ogre::Viewport *m_viewport;
};

}

}

#endif
