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

import strings = require("ui/EditorStrings");
import EditorUI = require("ui/EditorUI");
import MenuItemSources = require("./MenuItemSources");
import ServiceLocator from "../../../hostExtensions/ServiceLocator";

class HierarchyFrameMenus extends Atomic.ScriptObject {

    contentFolder: string;

    private contextMenuItemSource: Atomic.UIMenuItemSource = null;

    constructor() {

        super();

        MenuItemSources.createMenuItemSource("hierarchy create items", createItems);
        this.contextMenuItemSource = MenuItemSources.createMenuItemSource("node context general", nodeGeneralContextItems);

        this.subscribeToEvent(Editor.ContentFolderChangedEvent((ev: Editor.ContentFolderChangedEvent) => {
            this.contentFolder = ev.path;
        }));

    }

	handleNodeContextMenu(target: Atomic.UIWidget, refid: string, node: Atomic.Node, editor: Editor.SceneEditor3D): boolean {

        if (target.id == "node context menu" || target.id == "create popup") {

            if (!node) 
                return false;
            
			if (target.id == "create popup")
				node = node.scene;


			var child: Atomic.Node;
			
            if (refid == "create_node") {

                if (node) {

                    child = node.createChild("Node");
                }

            }
            else if (refid == "create_Box" || refid == "create_Cone" || refid == "create_Cylinder" || refid == "create_Plane" || refid == "create_Sphere") {

                if (node) {
					
					var object = refid.split('_')[1];
					var cache = Atomic.getResourceCache();
					var model = <Atomic.Model>cache.getResource("Model", "Models/" + object + ".mdl");
                    child = node.createChild(object);
                    var staticModel = <Atomic.StaticModel>child.createComponent("StaticModel");
					staticModel.setModel(model);
					
                }

            }else if (refid == "create_Light") {

                if (node) {

                    child = node.createChild("Light");
                    child.createComponent("Light");

                }

            }
            else if (refid == "delete_node") {

                if (node instanceof Atomic.Scene)
                    return;

                var scene = node.scene;
                scene.sendEvent(Editor.SceneEditAddRemoveNodesEventData({ end: false }));
                scene.sendEvent(Editor.SceneEditNodeRemovedEventData({ node: node, parent: node.parent, scene: scene }));
                node.remove();
                scene.sendEvent(Editor.SceneEditAddRemoveNodesEventData({ end: true }));

                editor.selection.delete();

                return true;

            } else if (refid == "duplicate_node") {
				this.duplicate_node(node);
            }
			
			
            if (child) {
                child.scene.sendEvent(Editor.SceneEditNodeCreatedEventData({ node: child }));
				return true;
            }

            // Let plugins handle context
            return ServiceLocator.uiServices.hierarchyContextItemClicked(node, refid);
        }

        return false;

    }

    handlePopupMenu(target: Atomic.UIWidget, refid: string, node: Atomic.Node, editor: Editor.SceneEditor3D): boolean {

        if (!target || !refid) return false;

		if (this.handleNodeContextMenu(target, refid, node, editor)) {

            return true;

        }

        

        return false;

    }


    createNodeContextMenu(parent: Atomic.UIWidget, node: Atomic.Node, x: number, y: number) {


        var menu = new Atomic.UIMenuWindow(parent, "node context menu");

        menu["node"] = node;

        var srcName: string = "node context general";

        var src = MenuItemSources.getMenuItemSource(srcName);
        menu.show(src, x, y);


    }

	duplicate_node(node: Atomic.Node)
	{
		if (node instanceof Atomic.Scene || !node)
			return;
		
		var newnode = node.clone();
		node.scene.sendEvent(Editor.SceneEditNodeCreatedEventData({ node: newnode }));

		return true;
	}

    createPluginItemSource(id: string, items: any): Atomic.UIMenuItemSource {
        return MenuItemSources.createSubMenuItemSource(this.contextMenuItemSource , id, items);
    }

    removePluginItemSource(id: string) {
        this.contextMenuItemSource.removeItemWithStr(id);
    }

}

export = HierarchyFrameMenus;

// initialization
var StringID = strings.StringID;

var createItems = {
    "Node": ["create_node", undefined, "Folder.icon"],
    "-1": null,
    "3D": {
        "Box": ["create_Box", undefined, "Cube"],
        "Cone": ["create_Cone", undefined, "Cube"],
        "Cylinder": ["create_Cylinder", undefined, "Cube"],
        "Plane": ["create_Plane", undefined, "Cube"],
        "Sphere": ["create_Sphere", undefined, "Cube"],
		"-1": null,
        "Light": ["create_Light", undefined, "cube"]
    }
};

var nodeGeneralContextItems = {
	"Create": createItems,
	"-1": null,
    "Duplicate": ["duplicate_node", StringID.ShortcutDuplicateNode, ""],
    "-2": null,
    "Delete": ["delete_node", undefined, ""]
};
