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

#pragma once

#include "../Graphics/StaticModel.h"

namespace Atomic
{

class Node;
class Scene;

/// Water example.
/// This sample demonstrates:
///     - Creating a large plane to represent a water body for rendering
///     - Setting up a second camera to render reflections on the water surface

class ATOMIC_API Water : public StaticModel
{
    ATOMIC_OBJECT(Water, StaticModel);
	
public:
    /// Construct.
    explicit Water(Context* context);
	/// Destruct.
	~Water() override;
	/// Register object factory.
    static void RegisterObject(Context* context);
	
private:
	/// Handle scene node being assigned at creation.
	virtual void OnNodeSet(Node* node);
};

}
