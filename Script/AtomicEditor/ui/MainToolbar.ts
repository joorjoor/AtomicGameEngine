//
// Copyright (c) 2014-2016 THUNDERBEAST GAMES LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

import EditorUI = require("./EditorUI");

class MainToolbar extends Atomic.UIWidget {

    translateButton: Atomic.UIButton;
    rotateButton: Atomic.UIButton;
    scaleButton: Atomic.UIButton;
    axisButton: Atomic.UIButton;
    playButton: Atomic.UIButton;
    pauseButton: Atomic.UIButton;
    stepButton: Atomic.UIButton;
	fillSolidButton: Atomic.UIButton;
	fillWireFrameButton: Atomic.UIButton;
	fillPointButton: Atomic.UIButton;

    constructor(parent: Atomic.UIWidget) {

        super();

        this.load("AtomicEditor/editor/ui/maintoolbar.tb.txt");

        this.translateButton = <Atomic.UIButton>this.getWidget("3d_translate");
        this.rotateButton = <Atomic.UIButton>this.getWidget("3d_rotate");
        this.scaleButton = <Atomic.UIButton>this.getWidget("3d_scale");
		
        this.fillSolidButton = <Atomic.UIButton>this.getWidget("fillSolid");
        this.fillWireFrameButton = <Atomic.UIButton>this.getWidget("fillWireFrame");
        this.fillPointButton = <Atomic.UIButton>this.getWidget("fillPoint");

        this.axisButton = <Atomic.UIButton>this.getWidget("3d_axismode");

        this.playButton = <Atomic.UIButton>this.getWidget("maintoolbar_play");

        this.pauseButton = <Atomic.UIButton>this.getWidget("maintoolbar_pause");

        this.stepButton = <Atomic.UIButton>this.getWidget("maintoolbar_step");

        this.translateButton.value = 1;
		
        parent.addChild(this);

        this.subscribeToEvent(Editor.GizmoAxisModeChangedEvent((ev) => this.handleGizmoAxisModeChanged(ev)));
        this.subscribeToEvent(Editor.GizmoEditModeChangedEvent((ev) => this.handleGizmoEditModeChanged(ev)));

        this.subscribeToEvent(this, Atomic.UIWidgetEvent((data) => this.handleWidgetEvent(data)));

        this.subscribeToEvent(Editor.EditorPlayerStartedEvent(() => {
            var skin = <Atomic.UISkinImage> this.playButton.getWidget("skin_image");
            skin.setSkinBg("StopButton");
            skin = <Atomic.UISkinImage> this.pauseButton.getWidget("skin_image");
            skin.setSkinBg("PauseButton");
        }));
        this.subscribeToEvent(Editor.EditorPlayerStoppedEvent(() => {
            var skin = <Atomic.UISkinImage> this.playButton.getWidget("skin_image");
            skin.setSkinBg("PlayButton");
            skin = <Atomic.UISkinImage> this.pauseButton.getWidget("skin_image");
            skin.setSkinBg("PauseButton");
        }));
        this.subscribeToEvent(Editor.EditorPlayerPausedEvent(() => {
            var skin = <Atomic.UISkinImage> this.pauseButton.getWidget("skin_image");
            skin.setSkinBg("PlayButton");
        }));

        this.subscribeToEvent(Editor.EditorPlayerResumedEvent(() => {
            var skin = <Atomic.UISkinImage> this.pauseButton.getWidget("skin_image");
            skin.setSkinBg("PauseButton");
        }));

        // TODO: We need better control over playmode during NET compiles
        this.subscribeToEvent(ToolCore.NETBuildBeginEvent((data) => {
            this.playButton.disable();
        }));

        this.subscribeToEvent(ToolCore.NETBuildResultEvent((data) => {
            this.playButton.enable();
        }));
		
		this.subscribeToEvent(Editor.EditorProjectClosedEvent(() => this.refreshFillModeButtons(0)));

        this.subscribeToEvent(Editor.EditorSceneClosedEvent(() => this.refreshFillModeButtons(0)));
		
		this.subscribeToEvent(Editor.EditorResourceEditorChangedEvent(() => this.refreshFillModeButtons(4)));
		
		this.subscribeToEvent(Editor.EditorActiveSceneEditorChangeEvent(() => this.refreshFillModeButtons(1)));

    }

    handleGizmoAxisModeChanged(ev: Editor.GizmoAxisModeChangedEvent) {
        if (ev.mode) {
            this.axisButton.value = 0;
            this.axisButton.text = "Local";
        } else {
            this.axisButton.value = 1;
            this.axisButton.text = "World";
        }
    }

    handleGizmoEditModeChanged(ev: Editor.GizmoEditModeChangedEvent) {

        this.translateButton.value = 0;
        this.rotateButton.value = 0;
        this.scaleButton.value = 0;

        switch (ev.mode) {
            case 1:
                this.translateButton.value = 1;
                break;
            case 2:
                this.rotateButton.value = 1;
                break;
            case 3:
                this.scaleButton.value = 1;
                break;
        }

    }

    handleWidgetEvent(ev: Atomic.UIWidgetEvent) {

        if (ev.type == Atomic.UI_EVENT_TYPE.UI_EVENT_TYPE_CLICK && ev.target) {

            if (ev.target.id == "3d_translate" || ev.target.id == "3d_rotate" || ev.target.id == "3d_scale") {

                var mode = 1;
                if (ev.target.id == "3d_rotate")
                    mode = 2;
                else if (ev.target.id == "3d_scale")
                    mode = 3;

                this.sendEvent(Editor.GizmoEditModeChangedEventData({ mode: mode }));

                return true;

            } 
			else if (ev.target.id == "fillSolid" || ev.target.id == "fillWireFrame" || ev.target.id == "fillPoint") {
				var editor = EditorUI.getCurrentResourceEditor();
				if (editor && editor instanceof Editor.SceneEditor3D)
					switch(ev.target.id)
					{
						case 'fillSolid':
							editor.getSceneView3D().getCameraEditor().setFillMode(Atomic.FillMode.FILL_SOLID);
							this.refreshFillModeButtons(1);
							break;
						case 'fillWireFrame':
							editor.getSceneView3D().getCameraEditor().setFillMode(Atomic.FillMode.FILL_WIREFRAME);
							this.refreshFillModeButtons(2);
							break;
						case 'fillPoint':
							editor.getSceneView3D().getCameraEditor().setFillMode(Atomic.FillMode.FILL_POINT);
							this.refreshFillModeButtons(3);
							break;
					}
				else
					this.refreshFillModeButtons(0);
                return true;

			} else if (ev.target.id == "3d_axismode") {

                EditorUI.getShortcuts().toggleGizmoAxisMode();
                return true;

            } else if (ev.target.id == "maintoolbar_play") {
                EditorUI.getShortcuts().invokePlayOrStopPlayer();
                return true;

            } else if (ev.target.id == "maintoolbar_pause") {
                EditorUI.getShortcuts().invokePauseOrResumePlayer();
                return true;

            } else if (ev.target.id == "maintoolbar_step") {
                EditorUI.getShortcuts().invokeStepPausedPlayer();
                return true;
            }

        }

    }
	
	refreshFillModeButtons(but: number)
	{
		this.fillSolidButton.value = 0;
		this.fillWireFrameButton.value = 0;
		this.fillPointButton.value = 0;
		switch(but)
		{
			case 1:
				this.fillSolidButton.value = 1;
				break;
			case 2:
				this.fillWireFrameButton.value = 1;
				break;
			case 3:
				this.fillPointButton.value = 1;
				break;
			case 4:
				var editor = EditorUI.getCurrentResourceEditor();
				if (editor && editor instanceof Editor.SceneEditor3D)
					switch(editor.getSceneView3D().getCameraEditor().getFillMode())
					{
						case Atomic.FillMode.FILL_SOLID:
							this.refreshFillModeButtons(1);
							break;
						case Atomic.FillMode.FILL_WIREFRAME:
							this.refreshFillModeButtons(2);
							break;
						case Atomic.FillMode.FILL_POINT:
							this.refreshFillModeButtons(3);
							break;
					}
				break;
		}
	}

}



export = MainToolbar;
