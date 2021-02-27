//
// Copyright (c) 2008-2020 the Urho3D project.
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
#include "../Atomic2D/CollisionCircle2D.h"
#include "../Atomic2D/PhysicsUtils2D.h"
//ATOMIC BEGIN
#include "../Graphics/DebugRenderer.h"
#include "../Scene/Node.h"
//ATOMIC END
#include "../DebugNew.h"

namespace Atomic
{

extern const char* ATOMIC2D_CATEGORY;
static const float DEFAULT_CLRCLE_RADIUS(0.01f);

CollisionCircle2D::CollisionCircle2D(Context* context) :
    CollisionShape2D(context),
    radius_(DEFAULT_CLRCLE_RADIUS),
    center_(Vector2::ZERO)
{
    circleShape_.m_radius = DEFAULT_CLRCLE_RADIUS * cachedWorldScale_.x_;
    fixtureDef_.shape = &circleShape_;
}

CollisionCircle2D::~CollisionCircle2D() = default;

void CollisionCircle2D::RegisterObject(Context* context)
{
    context->RegisterFactory<CollisionCircle2D>(ATOMIC2D_CATEGORY);

    ATOMIC_ACCESSOR_ATTRIBUTE("Is Enabled", IsEnabled, SetEnabled, bool, true, AM_DEFAULT);
    ATOMIC_ACCESSOR_ATTRIBUTE("Radius", GetRadius, SetRadius, float, DEFAULT_CLRCLE_RADIUS, AM_DEFAULT);
    ATOMIC_ACCESSOR_ATTRIBUTE("Center", GetCenter, SetCenter, Vector2, Vector2::ZERO, AM_DEFAULT);
    ATOMIC_COPY_BASE_ATTRIBUTES(CollisionShape2D);
}

void CollisionCircle2D::SetRadius(float radius)
{
    if (radius == radius_)
        return;

    radius_ = radius;

    RecreateFixture();
    MarkNetworkUpdate();
}

void CollisionCircle2D::SetCenter(const Vector2& center)
{
    if (center == center_)
        return;

    center_ = center;

    RecreateFixture();
    MarkNetworkUpdate();
}

void CollisionCircle2D::SetCenter(float x, float y)
{
    SetCenter(Vector2(x, y));
}

void CollisionCircle2D::ApplyNodeWorldScale()
{
    RecreateFixture();
}

void CollisionCircle2D::RecreateFixture()
{
    ReleaseFixture();

    // Only use scale in x axis for circle
    float worldScale = cachedWorldScale_.x_;
    circleShape_.m_radius = radius_ * worldScale;
    circleShape_.m_p = ToB2Vec2(center_ * worldScale);
    
	//ATOMIC BEGIN
    if (IsEnabledEffective())
    {
		CreateFixture();
	}
    //ATOMIC END
}

//ATOMIC BEGIN
void CollisionCircle2D::DrawDebugGeometry(DebugRenderer* debug, bool depthTest)
{
	if (debug && IsEnabledEffective())
	{	
		float radius = radius_ * cachedWorldScale_.x_;
		Vector3 pos_ = node_->GetWorldPosition() + Vector3(center_);
		Vector3 v1(pos_.x_, pos_.y_, 0.0f);
		debug->AddCircle(v1, Vector3::FORWARD, radius, Color::WHITE, 64, depthTest);
	}
}
//ATOMIC END

}
