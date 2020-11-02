//
// Copyright (c) 2020 the ATOMIC project.
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

#include "../Precompiled.h"

#include "../Core/Context.h"
#include "../Graphics/Camera.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/Model.h"
#include "../Graphics/Material.h"
#include "../Graphics/Water.h"
#include "../Scene/Scene.h"
#include "../Scene/Node.h"
#include "../Scene/SceneEvents.h"
#include "../Resource/ResourceCache.h"

#include "../Graphics/OctreeQuery.h"

#include "../DebugNew.h"


namespace Atomic
{

extern const char* GEOMETRY_CATEGORY;

Water::Water(Context* context) : 
	StaticModel(context)
{
}

Water::~Water() = default;

void Water::RegisterObject(Context* context)
{
	context->RegisterFactory<Water>(GEOMETRY_CATEGORY);

	ATOMIC_ACCESSOR_ATTRIBUTE("Is Enabled", IsEnabled, SetEnabled, bool, true, AM_DEFAULT);
}

void Water::OnNodeSet(Node* node)
{
	if (node)
	{
        node->AddListener(this);
		ResourceCache* cache = GetSubsystem<ResourceCache>();
		SetModel(cache->GetResource<Model>("Models/Plane.mdl"));
		SetMaterial(cache->GetResource<Material>("Materials/Water.xml"));
		SetViewMask(DRAWABLE_GEOMETRY);
	}
}

}