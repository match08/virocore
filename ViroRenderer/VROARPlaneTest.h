//
//  VROARPlaneTest.h
//  ViroKit
//
//  Created by Raj Advani on 10/1/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROARPlaneTest_h
#define VROARPlaneTest_h

#include "VRORendererTest.h"
#include "VROARDeclarativeNode.h"

class VROARPlaneTest : public VRORendererTest, public VROARDeclarativeNodeDelegate,
                       public std::enable_shared_from_this<VROARPlaneTest> {
public:
    
    VROARPlaneTest();
    virtual ~VROARPlaneTest();
    
    void build(std::shared_ptr<VRORenderer> renderer,
               std::shared_ptr<VROFrameSynchronizer> frameSynchronizer,
               std::shared_ptr<VRODriver> driver);
    std::shared_ptr<VRONode> getPointOfView() {
        return _pointOfView;
    }
    std::shared_ptr<VROSceneController> getSceneController() {
        return _sceneController;
    }
    
    void onARAnchorAttached(std::shared_ptr<VROARAnchor> anchor);
    void onARAnchorUpdated(std::shared_ptr<VROARAnchor> anchor);
    void onARAnchorRemoved();
    
private:

    std::shared_ptr<VRONode> _pointOfView;
    std::shared_ptr<VROSceneController> _sceneController;
    
};

#endif /* VROARPlaneTest_h */
