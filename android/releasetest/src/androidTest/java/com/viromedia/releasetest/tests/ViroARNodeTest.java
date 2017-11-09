package com.viromedia.releasetest.tests;

import android.graphics.Bitmap;
import android.graphics.Color;

import com.viro.renderer.jni.ARAnchor;
import com.viro.renderer.jni.ARNode;
import com.viro.renderer.jni.ARPlaneAnchor;
import com.viro.renderer.jni.ARScene;
import com.viro.renderer.jni.AmbientLight;
import com.viro.renderer.jni.Box;
import com.viro.renderer.jni.Material;
import com.viro.renderer.jni.Node;

import com.viro.renderer.jni.Surface;
import com.viro.renderer.jni.Text;
import com.viro.renderer.jni.Texture;
import com.viro.renderer.jni.Vector;

import org.junit.Test;

import java.util.Arrays;
import java.util.List;

/**
 * Created by vadvani on 11/7/17.
 * This includes tests for all methods for ARScene except the onAnchor callbacks. Those are tested in ViroARNodeTest.
 */

public class ViroARNodeTest extends ViroBaseTest {

    private ARScene mARScene;
    private Node mBoxNode;
    private AmbientLight mAmbientLight;
    private Text mTestText;

    private boolean mAnchorFoundTestStarted = false;
    private boolean mAnchorUpdatedTestStarted = false;
    private boolean mNodePauseUpdatesTestStarted = false;

    @Override
    void configureTestScene() {
        mAmbientLight = new AmbientLight(Color.WHITE, 1000.0f);
        mScene.getRootNode().addLight(mAmbientLight);
        mARScene = (ARScene)mScene;

        final Bitmap bobaBitmap = getBitmapFromAssets(mActivity, "boba.png");
        final Texture bobaTexture = new Texture(bobaBitmap, Texture.TextureFormat.RGBA8, true, true);
        final Material material = new Material();
        material.setDiffuseTexture(bobaTexture);
        material.setLightingModel(Material.LightingModel.BLINN);
        Box box = new Box(1, 1, 1);
        box.setMaterials(Arrays.asList(material));
        mBoxNode = new Node();
        mBoxNode.setGeometry(box);
        mBoxNode.setPosition(new Vector(0, 0, -4));
        //mScene.getRootNode().addChildNode(mBoxNode);

        mTestText = new Text(mViroView.getViroContext(),
                "No input yet.", "Roboto", 12, Color.WHITE, 1f, 1f, Text.HorizontalAlignment.LEFT,
                Text.VerticalAlignment.TOP, Text.LineBreakMode.WORD_WRAP, Text.ClipMode.CLIP_TO_BOUNDS, 0);

        Node textNode = new Node();
        textNode.setPosition(new Vector(0, -1f, -4f));
        textNode.setGeometry(mTestText);
        mScene.getRootNode().addChildNode(textNode);

        mARScene.setDelegate(new ARScene.Delegate() {
            @Override
            public void onTrackingInitialized() {
                mTestText.setText("AR Initialized callback received!");
            }

            @Override
            public void onAmbientLightUpdate(float lightIntensity, float colorTemperature) {

            }

            @Override
            public void onAnchorFound(ARAnchor anchor, ARNode node) {
                if(mAnchorFoundTestStarted) {
                    Text text  = new Text(mViroView.getViroContext(),
                            "Text attached to ARNODE!", "Roboto", 12, Color.WHITE, 1f, 1f, Text.HorizontalAlignment.LEFT,
                            Text.VerticalAlignment.TOP, Text.LineBreakMode.WORD_WRAP, Text.ClipMode.NONE, 0);
                    Node textNode = new Node();
                    textNode.setGeometry(text);
                    textNode.setPosition(new Vector(0f, .2f, 0f));

                    Material material = new Material();
                    material.setDiffuseColor(Color.RED);
                    material.setLightingModel(Material.LightingModel.BLINN);
                    if(anchor instanceof ARPlaneAnchor) {
                        ARPlaneAnchor arPlaneAnchor  = (ARPlaneAnchor)anchor;
                        Surface surface = new Surface(arPlaneAnchor.getExtent().x, arPlaneAnchor.getExtent().z);
                        surface.setMaterials(Arrays.asList(material));

                        Node nodeSurface = new Node();
                        node.setGeometry(surface);
                        nodeSurface.setPosition(new Vector(0, 0, 0));
                        nodeSurface.setRotation(new Vector(-Math.toRadians(90.0), 0, 0));
                        node.addChildNode(nodeSurface);
                    }
                    node.addChildNode(textNode);
                }
            }

            @Override
            public void onAnchorUpdated(ARAnchor anchor, ARNode node) {
                if(mAnchorUpdatedTestStarted) {
                    List<Node> childNodes = node.getChildNodes();
                    for (Node childNode : childNodes) {
                        if(childNode.getGeometry() instanceof Text) {
                            Text text = (Text) childNode.getGeometry();
                            String anchorString = "";

                            anchorString = "Anchor Type:" + anchor.getType().toString() + ", pos:" + anchor.getPosition().toString() + ", rotation: " + anchor.getRotation().toString() + "scale: " + anchor.getScale().toString();
                            if (anchor instanceof ARPlaneAnchor) {
                                ARPlaneAnchor anchorPlane = (ARPlaneAnchor) anchor;
                                anchorString += ("Center:" + anchorPlane.getCenter().toString() + ", " + "Extent: " + anchorPlane.getExtent().toString() + ", Alignment: " + anchorPlane.getAlignment().toString());
                            }
                            text.setText(anchorString);
                        }

                        if(childNode.getGeometry() instanceof Surface) {
                            ARPlaneAnchor planeAnchor = (ARPlaneAnchor)anchor;
                            Surface surface = (Surface)childNode.getGeometry();
                            surface.setWidth(planeAnchor.getExtent().x);
                            surface.setHeight(planeAnchor.getExtent().z);
                            childNode.setScale(planeAnchor.getScale());
                        }
                    }
                }

                if(mNodePauseUpdatesTestStarted) {
                    node.setPauseUpdates(true);
                }
            }

            @Override
            public void onAnchorRemoved(ARAnchor anchor, ARNode node) {
               mTestText.setText("An anchor has been removed!");
            }
        });
    }

    @Test
    public void testARScene() {
        testOnAnchorFound();
        testOnAnchorUpdated();
        testPauseUpdates();
        testOnAnchorRemoved();
    }


    private void testOnAnchorFound() {
        mAnchorFoundTestStarted = true;
        assertPass("Anchors with ARNodes are found with text displaying above them. Red Spheres display for anchor points and red squares for planes.", ()-> {
            mAnchorFoundTestStarted = false;
        });
    }

    private void testOnAnchorUpdated() {
        mAnchorUpdatedTestStarted = true;
        assertPass("Anchors are updated with anchor info showing up in text.", ()-> {
            mAnchorUpdatedTestStarted = false;
        });
    }

    private void testOnAnchorRemoved() {
        mAnchorUpdatedTestStarted = true;
        assertPass("Move around till anchor removed message appears where init message appeared. Plane should disappear.");
    }

    private void testPauseUpdates() {
        mNodePauseUpdatesTestStarted = true;
        assertPass("Anchors are paused, no updates should be occurring to the node attached to the anchor");
    }

}