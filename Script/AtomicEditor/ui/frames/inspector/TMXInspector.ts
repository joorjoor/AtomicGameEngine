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

import InspectorWidget = require("./InspectorWidget");
import InspectorUtils = require("./InspectorUtils");
import EditorUI = require("ui/EditorUI");

class TMXInspector extends InspectorWidget {

    constructor() {

        super();

        this.subscribeToEvent(this, Atomic.UIWidgetEvent((data) => this.handleWidgetEvent(data)));

    }

    handleWidgetEvent(ev: Atomic.UIWidgetEvent):boolean {

      return false;

    }

    inspect(asset: ToolCore.Asset) {

        this.asset = asset;
        this.importer = <ToolCore.ModelImporter> asset.importer;

        // node attr layout
        var rootLayout = this.rootLayout;

        // Model Section
        var modelLayout = this.createSection(rootLayout, "TileMap2D", 1);

		    var editField = InspectorUtils.createAttrEditField("Name", modelLayout);

        var lp = new Atomic.UILayoutParams();
        editField.readOnly = true;
        editField.text = asset.name;

        //This should preferably be onClick
        editField.subscribeToEvent(editField, Atomic.UIWidgetFocusChangedEvent((ev: Atomic.UIWidgetFocusChangedEvent) => {

            if (ev.widget == editField && editField.focus) {
                this.sendEvent(Editor.InspectorProjectReferenceEventData({ "path": asset.getRelativePath() }));
            }

        }));

        var button = new Atomic.UIButton();
        button.fontDescription = this.attrFontDesc;
        button.gravity = Atomic.UI_GRAVITY.UI_GRAVITY_RIGHT;
        button.text = "Edit TileMap2D";

        button.onClick = function () {
            //this.sendMessage("Use Tiled Map Editor (https://www.mapeditor.org/)");
            //var mess = new Atomic.UIMessageWindow(new Atomic.UIView(), "modal_warning");
            //mess.show("Tiled Map Editor", "Use Tiled Map Editor (https://www.mapeditor.org/)", Atomic.UI_MESSAGEWINDOW_SETTINGS.UI_MESSAGEWINDOW_SETTINGS_OK, 0, 0, 0);
            EditorUI.getModelOps().showError("Tiled Map Editor", "Use Tiled Map Editor (https://www.mapeditor.org/)");
            // button is deleted in callback, so make sure we return
            // that we're handled
            return true;
        }.bind(this);


        rootLayout.addChild(button);

    }

    // TileMap2D

    asset: ToolCore.Asset;
    importer: ToolCore.ModelImporter;

}

export = TMXInspector;
