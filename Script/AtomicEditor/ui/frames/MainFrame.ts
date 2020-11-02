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

import ProjectFrame = require("./ProjectFrame");
import ResourceFrame = require("./ResourceFrame");
import WelcomeFrame = require("./WelcomeFrame");
import InspectorFrame = require("./inspector/InspectorFrame");
import HierarchyFrame = require("./HierarchyFrame");
import MainToolbar = require("ui/MainToolbar");
import AnimationToolbar = require("ui/AnimationToolbar");

import ScriptWidget = require("ui/ScriptWidget");
import MainFrameMenu = require("./menus/MainFrameMenu");

import MenuItemSources = require("./menus/MenuItemSources");

class MainFrame extends ScriptWidget {

    constructor() {

        super();

        this.load("AtomicEditor/editor/ui/mainframe.tb.txt");

        this.inspectorlayout = <Atomic.UILayout> this.getWidget("inspectorlayout");

        this.getWidget("consolecontainer").visibility = Atomic.UI_WIDGET_VISIBILITY.UI_WIDGET_VISIBILITY_GONE;

        this.statusText = <Atomic.UIEditField>this.getWidget("editorStatusText");
        this.statusText.subscribeToEvent(Atomic.UpdateEvent((ev) => this.handleStatusAging(ev)));

        this.inspectorframe = new InspectorFrame();
        this.inspectorlayout.addChild(this.inspectorframe);

        this.projectframe = new ProjectFrame(this);
        this.hierarchyFrame = new HierarchyFrame(this);

        this.welcomeFrame = new WelcomeFrame(this);
        this.resourceframe = new ResourceFrame(this);

        this.mainToolbar = new MainToolbar(this.getWidget("maintoolbarcontainer"));

        this.menu = new MainFrameMenu();

        this.disableProjectMenus();

        this.subscribeToEvent(Editor.EditorResourceEditorChangedEvent((data) => this.handleResourceEditorChanged(data)));

        this.subscribeToEvent(ToolCore.ProjectLoadedEvent((data) => {
            this.showWelcomeFrame(false);
            this.enableProjectMenus();
        }));

        this.subscribeToEvent(Editor.ProjectUnloadedNotificationEvent((data) => {
            this.showWelcomeFrame(true);
            this.disableProjectMenus();
        }));

        this.showWelcomeFrame(true);

    }

    frameVisible(frame: Atomic.UIWidget): boolean {

        var container = <Atomic.UILayout> this.getWidget("resourceviewcontainer");

        var child = null;
        for (child = container.firstChild; child; child = child.next) {
            if (child == frame)
                return true;
        }

        return false;
    }

    showWelcomeFrame(show: boolean) {

        if (show) {
            this.showInspectorFrame(false);
            this.welcomeFrame.visibility = Atomic.UI_WIDGET_VISIBILITY.UI_WIDGET_VISIBILITY_VISIBLE;
            this.resourceframe.visibility = Atomic.UI_WIDGET_VISIBILITY.UI_WIDGET_VISIBILITY_GONE;
        }
        else {
            this.showInspectorFrame(true);
            this.resourceframe.visibility = Atomic.UI_WIDGET_VISIBILITY.UI_WIDGET_VISIBILITY_VISIBLE;
            this.welcomeFrame.visibility = Atomic.UI_WIDGET_VISIBILITY.UI_WIDGET_VISIBILITY_GONE;
        }

    }

    showInspectorFrame(show: boolean) {

        if (show) {

            this.inspectorlayout.visibility = Atomic.UI_WIDGET_VISIBILITY.UI_WIDGET_VISIBILITY_VISIBLE;
            this.inspectorframe.visibility = Atomic.UI_WIDGET_VISIBILITY.UI_WIDGET_VISIBILITY_VISIBLE;

        } else {

            this.inspectorframe.visibility = Atomic.UI_WIDGET_VISIBILITY.UI_WIDGET_VISIBILITY_GONE;
            this.inspectorlayout.visibility = Atomic.UI_WIDGET_VISIBILITY.UI_WIDGET_VISIBILITY_GONE;

        }

    }

    onEventClick(target: Atomic.UIWidget, refid: string): boolean {

        if (this.menu.handlePopupMenu(target, refid))
            return true;

        var src = MenuItemSources.getMenuItemSource(target.id);

        if (src) {

            var menu = new Atomic.UIMenuWindow(target, target.id + " popup");
            menu.show(src);
            return true;

        }

        return false;

    }

    disableProjectMenus() {
        this.getWidget("menu edit").setStateRaw(Atomic.UI_WIDGET_STATE.UI_WIDGET_STATE_DISABLED);
        this.getWidget("menu build").setStateRaw(Atomic.UI_WIDGET_STATE.UI_WIDGET_STATE_DISABLED);
    }

    enableProjectMenus() {
        this.getWidget("menu edit").setStateRaw(Atomic.UI_WIDGET_STATE.UI_WIDGET_STATE_NONE);
        this.getWidget("menu build").setStateRaw(Atomic.UI_WIDGET_STATE.UI_WIDGET_STATE_NONE);
    }

    shutdown() {

        this.resourceframe.shutdown();
        this.deleteAllChildren();

    }

    handleResourceEditorChanged(data: Editor.EditorResourceEditorChangedEvent): void {

        var editor = data.resourceEditor;

        if (editor) {

            //this.showInspectorFrame(editor.requiresInspector());

        } else {

            //this.showInspectorFrame(false);

        }

    }

    showAnimationToolbar(asset: ToolCore.Asset) {
        if (this.animationToolbar != null) {
            this.animationToolbar.closeViewer();
            this.animationToolbar = new AnimationToolbar(this.getWidget("animationtoolbarcontainer"), this.getWidget("animationpropertiescontainer"), asset);
        }
        else {
            this.animationToolbar = new AnimationToolbar(this.getWidget("animationtoolbarcontainer"), this.getWidget("animationpropertiescontainer"), asset);
        }
    }

    showStatusText(message: string) {
        this.statusText.text = message;  // display the status message
        this.updateDelta = 10.0;         // start the clock for removing the text
    }

    handleStatusAging(ev) {    // to avoid showing stale status text
        if ( this.updateDelta > 0 ) {
           this.updateDelta -= ev.timeStep;
           if ( this.updateDelta < 0 )
                this.statusText.text = "";  // remove it after 10 seconds.
        }
    }


    projectframe: ProjectFrame;
    resourceframe: ResourceFrame;
    inspectorframe: InspectorFrame;
    hierarchyFrame: HierarchyFrame;
    welcomeFrame: WelcomeFrame;
    inspectorlayout: Atomic.UILayout;
    mainToolbar: MainToolbar;
    animationToolbar: AnimationToolbar;
    menu: MainFrameMenu;
    statusText: Atomic.UIEditField;
    updateDelta: number;
}

export = MainFrame;
