#ifndef __app_hpp__
#define __app_hpp__

// needs tree lib

namespace app
{
  class CScene
  {
    protected:
    tree::COcTree* mStorage;
    
    public:
    void insert() { }
  };
  
  class IEntity { };
  class IEvent : public IEntity { };
  class IDrawable : public IEntity { };
  class IForce : public IEntity { };
  
  class CAreaEvent : public IEvent { };
  class CTimeEvent : public IEvent { };
  
  class CObject : public IDrawable { };
  class CDynamicObject : public CObject { };
  class CStaticObject : public CObject { };
  
  class CGravityForce : public IForce { };
  
  
  class CQuery // scene query
  {
  
  };
  
  class CResult
  {
    
  };

  class CGameApp : public core::CApp
  {
    protected:
    CScene* mScene;
    
    ogl::CRenderEngine*  mRenderEngine;
    oal::CAudioEngine*   mAudioEngine;
    phx::CPhysicsEngine* mPhysicsEngine;
    
    public:
    void onInit()
    {
      // @todo:
      // load objects into scene
    }
    
    void onUpdate()
    {
      // @todo:
      // update scene
        // update scene OcTree structure
        // for each entity inside the scene
          // update position
          // update animation
        // check for collisions
          // trigger collision event
    }
    
    void onRender()
    {
      // @todo:
      // query scene for drawables using current camera
        // draw drawable
    }
  };
}

#endif // __app_hpp__
