//
// Copyright (c) 2008-2017 the Urho3D project.
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

#include "../Container/HashMap.h"
#include "../Container/Ptr.h"
#include "../Math/Color.h"
#include "../Math/Matrix3.h"
#include "../Math/Matrix3x4.h"
#include "../Math/Rect.h"
#include "../Math/StringHash.h"

namespace Atomic
{

/// Variant's supported types.
enum VariantType
{
    VAR_NONE = 0,
    VAR_INT,
    VAR_BOOL,
    VAR_FLOAT,
    VAR_VECTOR2,
    VAR_VECTOR3,
    VAR_VECTOR4,
    VAR_QUATERNION,
    VAR_COLOR,
    VAR_STRING,
    VAR_BUFFER,
    VAR_VOIDPTR,
    VAR_RESOURCEREF,
    VAR_RESOURCEREFLIST,
    VAR_VARIANTVECTOR,
    VAR_VARIANTMAP,
    VAR_INTRECT,
    VAR_INTVECTOR2,
    VAR_PTR,
    VAR_MATRIX3,
    VAR_MATRIX3X4,
    VAR_MATRIX4,
    VAR_DOUBLE,
    VAR_STRINGVECTOR,
    VAR_RECT,
    VAR_INTVECTOR3,
    VAR_INT64,
	// Add new types here
    VAR_CUSTOM_HEAP,
    VAR_CUSTOM_STACK,
    MAX_VAR_TYPES

};

/// Union for the possible variant values. Also stores non-POD objects such as String and math objects (excluding Matrix) which must not exceed 16 bytes in size (or 32 bytes in a 64-bit build.) Objects exceeding the limit are allocated on the heap and pointed to by _ptr.
struct VariantValue
{
    union
    {
        int int_;
        bool bool_;
        float float_;
        void* ptr_;
    };

    union
    {
        int int2_;
        float float2_;
        void* ptr2_;
    };

    union
    {
        int int3_;
        float float3_;
        void* ptr3_;
    };

    union
    {
        int int4_;
        float float4_;
        void* ptr4_;
    };
};

class Variant;
class VectorBuffer;

/// Vector of variants.
typedef Vector<Variant> VariantVector;

/// Vector of strings.
typedef Vector<String> StringVector;

/// Map of variants.
typedef HashMap<StringHash, Variant> VariantMap;

/// Typed resource reference.
struct ATOMIC_API ResourceRef
{
    /// Construct.
    ResourceRef()
    {
    }

    /// Construct with type only and empty id.
    ResourceRef(StringHash type) :
        type_(type)
    {
    }

    /// Construct with type and resource name.
    ResourceRef(StringHash type, const String& name) :
        type_(type),
        name_(name)
    {
    }

    /// Construct with type and resource name.
    ResourceRef(const String& type, const String& name) :
        type_(type),
        name_(name)
    {
    }

    /// Construct with type and resource name.
    ResourceRef(const char* type, const char* name) :
        type_(type),
        name_(name)
    {
    }

    /// Construct from another ResourceRef.
    ResourceRef(const ResourceRef& rhs) :
        type_(rhs.type_),
        name_(rhs.name_)
    {
    }

    /// Object type.
    StringHash type_;
    /// Object name.
    String name_;

    /// Test for equality with another reference.
    bool operator ==(const ResourceRef& rhs) const { return type_ == rhs.type_ && name_ == rhs.name_; }

    /// Test for inequality with another reference.
    bool operator !=(const ResourceRef& rhs) const { return type_ != rhs.type_ || name_ != rhs.name_; }
};

/// %List of typed resource references.
struct ATOMIC_API ResourceRefList
{
    /// Construct.
    ResourceRefList()
    {
    }

    /// Construct with type only.
    ResourceRefList(StringHash type) :
        type_(type)
    {
    }

    /// Construct with type and id list.
    ResourceRefList(StringHash type, const StringVector& names) :
        type_(type),
        names_(names)
    {
    }

    /// Object type.
    StringHash type_;
    /// List of object names.
    StringVector names_;

    /// Test for equality with another reference list.
    bool operator ==(const ResourceRefList& rhs) const { return type_ == rhs.type_ && names_ == rhs.names_; }

    /// Test for inequality with another reference list.
    bool operator !=(const ResourceRefList& rhs) const { return type_ != rhs.type_ || names_ != rhs.names_; }
};

/// Variable that supports a fixed set of types.
class ATOMIC_API Variant
{
public:
    /// Construct empty.
    Variant() :
        type_(VAR_NONE)
    {
    }

    /// Construct from integer.
    Variant(int value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from 64 bit integer.
    Variant(long long value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from unsigned integer.
    Variant(unsigned value) :
        type_(VAR_NONE)
    {
        *this = (int)value;
    }

    /// Construct from unsigned integer.
    Variant(unsigned long long value) :
        type_(VAR_NONE)
    {
        *this = (long long)value;
    }

    /// Construct from a string hash (convert to integer).
    Variant(const StringHash& value) :
        type_(VAR_NONE)
    {
        *this = (int)value.Value();
    }

    /// Construct from a bool.
    Variant(bool value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a float.
    Variant(float value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a double.
    Variant(double value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a Vector2.
    Variant(const Vector2& value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a Vector3.
    Variant(const Vector3& value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a Vector4.
    Variant(const Vector4& value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a quaternion.
    Variant(const Quaternion& value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a color.
    Variant(const Color& value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a string.
    Variant(const String& value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a C string.
    Variant(const char* value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a buffer.
    Variant(const PODVector<unsigned char>& value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a %VectorBuffer and store as a buffer.
    Variant(const VectorBuffer& value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a pointer.
    Variant(void* value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a resource reference.
    Variant(const ResourceRef& value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a resource reference list.
    Variant(const ResourceRefList& value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a variant vector.
    Variant(const VariantVector& value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a variant map.
    Variant(const VariantMap& value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a string vector.
    Variant(const StringVector& value) :
        type_ (VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a rect.
    Variant(const Rect& value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from an integer rect.
    Variant(const IntRect& value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from an IntVector2.
    Variant(const IntVector2& value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from an IntVector3.
    Variant(const IntVector3& value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a RefCounted pointer. The object will be stored internally in a WeakPtr so that its expiration can be detected safely.
    Variant(RefCounted* value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a Matrix3.
    Variant(const Matrix3& value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a Matrix3x4.
    Variant(const Matrix3x4& value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a Matrix4.
    Variant(const Matrix4& value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from type and value.
    Variant(const String& type, const String& value) :
        type_(VAR_NONE)
    {
        FromString(type, value);
    }

    /// Construct from type and value.
    Variant(VariantType type, const String& value) :
        type_(VAR_NONE)
    {
        FromString(type, value);
    }

    /// Construct from type and value.
    Variant(const char* type, const char* value) :
        type_(VAR_NONE)
    {
        FromString(type, value);
    }

    /// Construct from type and value.
    Variant(VariantType type, const char* value) :
        type_(VAR_NONE)
    {
        FromString(type, value);
    }

    /// Copy-construct from another variant.
    Variant(const Variant& value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Destruct.
    ~Variant()
    {
        SetType(VAR_NONE);
    }

    /// Reset to empty.
    void Clear()
    {
        SetType(VAR_NONE);
    }

    /// Assign from another variant.
    Variant& operator =(const Variant& rhs);

    /// Assign from an integer.
    Variant& operator =(int rhs)
    {
        SetType(VAR_INT);
        value_.int_ = rhs;
        return *this;
    }

    /// Assign from 64 bit integer.
    Variant& operator =(long long rhs)
    {
        SetType(VAR_INT64);
        *reinterpret_cast<long long*>(&value_) = rhs;
        return *this;
    }

    /// Assign from unsigned 64 bit integer.
    Variant& operator =(unsigned long long rhs)
    {
        SetType(VAR_INT64);
        *reinterpret_cast<long long*>(&value_) = (long long)rhs;
        return *this;
    }

    /// Assign from an unsigned integer.
    Variant& operator =(unsigned rhs)
    {
        SetType(VAR_INT);
        value_.int_ = (int)rhs;
        return *this;
    }

    /// Assign from a StringHash (convert to integer.)
    Variant& operator =(const StringHash& rhs)
    {
        SetType(VAR_INT);
        value_.int_ = (int)rhs.Value();
        return *this;
    }

    /// Assign from a bool.
    Variant& operator =(bool rhs)
    {
        SetType(VAR_BOOL);
        value_.bool_ = rhs;
        return *this;
    }

    /// Assign from a float.
    Variant& operator =(float rhs)
    {
        SetType(VAR_FLOAT);
        value_.float_ = rhs;
        return *this;
    }

    /// Assign from a double.
    Variant& operator = (double rhs)
    {
        SetType(VAR_DOUBLE);
        *(reinterpret_cast<double*>(&value_)) = rhs;
        return *this;
    }

    /// Assign from a Vector2.
    Variant& operator =(const Vector2& rhs)
    {
        SetType(VAR_VECTOR2);
        *(reinterpret_cast<Vector2*>(&value_)) = rhs;
        return *this;
    }

    /// Assign from a Vector3.
    Variant& operator =(const Vector3& rhs)
    {
        SetType(VAR_VECTOR3);
        *(reinterpret_cast<Vector3*>(&value_)) = rhs;
        return *this;
    }

    /// Assign from a Vector4.
    Variant& operator =(const Vector4& rhs)
    {
        SetType(VAR_VECTOR4);
        *(reinterpret_cast<Vector4*>(&value_)) = rhs;
        return *this;
    }

    /// Assign from a quaternion.
    Variant& operator =(const Quaternion& rhs)
    {
        SetType(VAR_QUATERNION);
        *(reinterpret_cast<Quaternion*>(&value_)) = rhs;
        return *this;
    }

    /// Assign from a color.
    Variant& operator =(const Color& rhs)
    {
        SetType(VAR_COLOR);
        *(reinterpret_cast<Color*>(&value_)) = rhs;
        return *this;
    }

    /// Assign from a string.
    Variant& operator =(const String& rhs)
    {
        SetType(VAR_STRING);
        *(reinterpret_cast<String*>(&value_)) = rhs;
        return *this;
    }

    /// Assign from a C string.
    Variant& operator =(const char* rhs)
    {
        SetType(VAR_STRING);
        *(reinterpret_cast<String*>(&value_)) = String(rhs);
        return *this;
    }

    /// Assign from a buffer.
    Variant& operator =(const PODVector<unsigned char>& rhs)
    {
        SetType(VAR_BUFFER);
        *(reinterpret_cast<PODVector<unsigned char>*>(&value_)) = rhs;
        return *this;
    }

    /// Assign from a %VectorBuffer and store as a buffer.
    Variant& operator =(const VectorBuffer& rhs);

    /// Assign from a void pointer.
    Variant& operator =(void* rhs)
    {
        SetType(VAR_VOIDPTR);
        value_.ptr_ = rhs;
        return *this;
    }

    /// Assign from a resource reference.
    Variant& operator =(const ResourceRef& rhs)
    {
        SetType(VAR_RESOURCEREF);
        *(reinterpret_cast<ResourceRef*>(&value_)) = rhs;
        return *this;
    }

    /// Assign from a resource reference list.
    Variant& operator =(const ResourceRefList& rhs)
    {
        SetType(VAR_RESOURCEREFLIST);
        *(reinterpret_cast<ResourceRefList*>(&value_)) = rhs;
        return *this;
    }

    /// Assign from a variant vector.
    Variant& operator =(const VariantVector& rhs)
    {
        SetType(VAR_VARIANTVECTOR);
        *(reinterpret_cast<VariantVector*>(&value_)) = rhs;
        return *this;
    }

    /// Assign from a string vector.
    Variant& operator =(const StringVector& rhs)
    {
        SetType(VAR_STRINGVECTOR);
        *(reinterpret_cast<StringVector*>(&value_)) = rhs;
        return *this;
    }

    /// Assign from a variant map.
    Variant& operator =(const VariantMap& rhs)
    {
        SetType(VAR_VARIANTMAP);
        *(reinterpret_cast<VariantMap*>(&value_)) = rhs;
        return *this;
    }

    /// Assign from a rect.
    Variant& operator =(const Rect& rhs)
    {
        SetType(VAR_RECT);
        *(reinterpret_cast<Rect*>(&value_)) = rhs;
        return *this;
    }

    /// Assign from an integer rect.
    Variant& operator =(const IntRect& rhs)
    {
        SetType(VAR_INTRECT);
        *(reinterpret_cast<IntRect*>(&value_)) = rhs;
        return *this;
    }

    /// Assign from an IntVector2.
    Variant& operator =(const IntVector2& rhs)
    {
        SetType(VAR_INTVECTOR2);
        *(reinterpret_cast<IntVector2*>(&value_)) = rhs;
        return *this;
    }

    /// Assign from an IntVector3.
    Variant& operator =(const IntVector3& rhs)
    {
        SetType(VAR_INTVECTOR3);
        *(reinterpret_cast<IntVector3*>(&value_)) = rhs;
        return *this;
    }

    /// Assign from a RefCounted pointer. The object will be stored internally in a WeakPtr so that its expiration can be detected safely.
    Variant& operator =(RefCounted* rhs)
    {
        SetType(VAR_PTR);
        *(reinterpret_cast<WeakPtr<RefCounted>*>(&value_)) = rhs;
        return *this;
    }

    /// Assign from a Matrix3.
    Variant& operator =(const Matrix3& rhs)
    {
        SetType(VAR_MATRIX3);
        *(reinterpret_cast<Matrix3*>(value_.ptr_)) = rhs;
        return *this;
    }

    /// Assign from a Matrix3x4.
    Variant& operator =(const Matrix3x4& rhs)
    {
        SetType(VAR_MATRIX3X4);
        *(reinterpret_cast<Matrix3x4*>(value_.ptr_)) = rhs;
        return *this;
    }

    /// Assign from a Matrix4.
    Variant& operator =(const Matrix4& rhs)
    {
        SetType(VAR_MATRIX4);
        *(reinterpret_cast<Matrix4*>(value_.ptr_)) = rhs;
        return *this;
    }

    /// Test for equality with another variant.
    bool operator ==(const Variant& rhs) const;

    /// Test for equality with an integer. To return true, both the type and value must match.
    bool operator ==(int rhs) const { return type_ == VAR_INT ? value_.int_ == rhs : false; }

    /// Test for equality with an unsigned 64 bit integer. To return true, both the type and value must match.
    bool operator ==(unsigned rhs) const { return type_ == VAR_INT ? value_.int_ == (int)rhs : false; }

    /// Test for equality with an 64 bit integer. To return true, both the type and value must match.
    bool operator ==(long long rhs) const { return type_ == VAR_INT64 ? *reinterpret_cast<const long long*>(&value_.int_) == rhs : false; }

    /// Test for equality with an unsigned integer. To return true, both the type and value must match.
    bool operator ==(unsigned long long rhs) const { return type_ == VAR_INT64 ? *reinterpret_cast<const unsigned long long*>(&value_.int_) == (int)rhs : false; }

    /// Test for equality with a bool. To return true, both the type and value must match.
    bool operator ==(bool rhs) const { return type_ == VAR_BOOL ? value_.bool_ == rhs : false; }

    /// Test for equality with a float. To return true, both the type and value must match.
    bool operator ==(float rhs) const { return type_ == VAR_FLOAT ? value_.float_ == rhs : false; }

    /// Test for equality with a double. To return true, both the type and value must match.
    bool operator ==(double rhs) const { return type_ == VAR_DOUBLE ? *(reinterpret_cast<const double*>(&value_)) == rhs : false; }

    /// Test for equality with a Vector2. To return true, both the type and value must match.
    bool operator ==(const Vector2& rhs) const
    {
        return type_ == VAR_VECTOR2 ? *(reinterpret_cast<const Vector2*>(&value_)) == rhs : false;
    }

    /// Test for equality with a Vector3. To return true, both the type and value must match.
    bool operator ==(const Vector3& rhs) const
    {
        return type_ == VAR_VECTOR3 ? *(reinterpret_cast<const Vector3*>(&value_)) == rhs : false;
    }

    /// Test for equality with a Vector4. To return true, both the type and value must match.
    bool operator ==(const Vector4& rhs) const
    {
        return type_ == VAR_VECTOR4 ? *(reinterpret_cast<const Vector4*>(&value_)) == rhs : false;
    }

    /// Test for equality with a quaternion. To return true, both the type and value must match.
    bool operator ==(const Quaternion& rhs) const
    {
        return type_ == VAR_QUATERNION ? *(reinterpret_cast<const Quaternion*>(&value_)) == rhs : false;
    }

    /// Test for equality with a color. To return true, both the type and value must match.
    bool operator ==(const Color& rhs) const
    {
        return type_ == VAR_COLOR ? *(reinterpret_cast<const Color*>(&value_)) == rhs : false;
    }

    /// Test for equality with a string. To return true, both the type and value must match.
    bool operator ==(const String& rhs) const
    {
        return type_ == VAR_STRING ? *(reinterpret_cast<const String*>(&value_)) == rhs : false;
    }

    /// Test for equality with a buffer. To return true, both the type and value must match.
    bool operator ==(const PODVector<unsigned char>& rhs) const;
    /// Test for equality with a %VectorBuffer. To return true, both the type and value must match.
    bool operator ==(const VectorBuffer& rhs) const;

    /// Test for equality with a void pointer. To return true, both the type and value must match, with the exception that a RefCounted pointer is also allowed.
    bool operator ==(void* rhs) const
    {
        if (type_ == VAR_VOIDPTR)
            return value_.ptr_ == rhs;
        else if (type_ == VAR_PTR)
            return *(reinterpret_cast<const WeakPtr<RefCounted>*>(&value_)) == rhs;
        else
            return false;
    }

    /// Test for equality with a resource reference. To return true, both the type and value must match.
    bool operator ==(const ResourceRef& rhs) const
    {
        return type_ == VAR_RESOURCEREF ? *(reinterpret_cast<const ResourceRef*>(&value_)) == rhs : false;
    }

    /// Test for equality with a resource reference list. To return true, both the type and value must match.
    bool operator ==(const ResourceRefList& rhs) const
    {
        return type_ == VAR_RESOURCEREFLIST ? *(reinterpret_cast<const ResourceRefList*>(&value_)) == rhs : false;
    }

    /// Test for equality with a variant vector. To return true, both the type and value must match.
    bool operator ==(const VariantVector& rhs) const
    {
        return type_ == VAR_VARIANTVECTOR ? *(reinterpret_cast<const VariantVector*>(&value_)) == rhs : false;
    }

    /// Test for equality with a string vector. To return true, both the type and value must match.
    bool operator ==(const StringVector& rhs) const
    {
        return type_ == VAR_STRINGVECTOR ? *(reinterpret_cast<const StringVector*>(&value_)) == rhs : false;
    }

    /// Test for equality with a variant map. To return true, both the type and value must match.
    bool operator ==(const VariantMap& rhs) const
    {
        return type_ == VAR_VARIANTMAP ? *(reinterpret_cast<const VariantMap*>(&value_)) == rhs : false;
    }

    /// Test for equality with a rect. To return true, both the type and value must match.
    bool operator ==(const Rect& rhs) const
    {
        return type_ == VAR_RECT ? *(reinterpret_cast<const Rect*>(&value_)) == rhs : false;
    }

    /// Test for equality with an integer rect. To return true, both the type and value must match.
    bool operator ==(const IntRect& rhs) const
    {
        return type_ == VAR_INTRECT ? *(reinterpret_cast<const IntRect*>(&value_)) == rhs : false;
    }

    /// Test for equality with an IntVector2. To return true, both the type and value must match.
    bool operator ==(const IntVector2& rhs) const
    {
        return type_ == VAR_INTVECTOR2 ? *(reinterpret_cast<const IntVector2*>(&value_)) == rhs : false;
    }

    /// Test for equality with an IntVector3. To return true, both the type and value must match.
    bool operator ==(const IntVector3& rhs) const
    {
        return type_ == VAR_INTVECTOR3 ? *(reinterpret_cast<const IntVector3*>(&value_)) == rhs : false;
    }

    /// Test for equality with a StringHash. To return true, both the type and value must match.
    bool operator ==(const StringHash& rhs) const { return type_ == VAR_INT ? (unsigned)value_.int_ == rhs.Value() : false; }

    /// Test for equality with a RefCounted pointer. To return true, both the type and value must match, with the exception that void pointer is also allowed.
    bool operator ==(RefCounted* rhs) const
    {
        if (type_ == VAR_PTR)
            return *(reinterpret_cast<const WeakPtr<RefCounted>*>(&value_)) == rhs;
        else if (type_ == VAR_VOIDPTR)
            return value_.ptr_ == rhs;
        else
            return false;
    }

    /// Test for equality with a Matrix3. To return true, both the type and value must match.
    bool operator ==(const Matrix3& rhs) const
    {
        return type_ == VAR_MATRIX3 ? *(reinterpret_cast<const Matrix3*>(value_.ptr_)) == rhs : false;
    }

    /// Test for equality with a Matrix3x4. To return true, both the type and value must match.
    bool operator ==(const Matrix3x4& rhs) const
    {
        return type_ == VAR_MATRIX3X4 ? *(reinterpret_cast<const Matrix3x4*>(value_.ptr_)) == rhs : false;
    }

    /// Test for equality with a Matrix4. To return true, both the type and value must match.
    bool operator ==(const Matrix4& rhs) const
    {
        return type_ == VAR_MATRIX4 ? *(reinterpret_cast<const Matrix4*>(value_.ptr_)) == rhs : false;
    }

    /// Test for inequality with another variant.
    bool operator !=(const Variant& rhs) const { return !(*this == rhs); }

    /// Test for inequality with an integer.
    bool operator !=(int rhs) const { return !(*this == rhs); }

    /// Test for inequality with an unsigned integer.
    bool operator !=(unsigned rhs) const { return !(*this == rhs); }

    /// Test for inequality with an 64 bit integer.
    bool operator !=(long long rhs) const { return !(*this == rhs); }

    /// Test for inequality with an unsigned 64 bit integer.
    bool operator !=(unsigned long long rhs) const { return !(*this == rhs); }

    /// Test for inequality with a bool.
    bool operator !=(bool rhs) const { return !(*this == rhs); }

    /// Test for inequality with a float.
    bool operator !=(float rhs) const { return !(*this == rhs); }

    /// Test for inequality with a double.
    bool operator !=(double rhs) const { return !(*this == rhs); }

    /// Test for inequality with a Vector2.
    bool operator !=(const Vector2& rhs) const { return !(*this == rhs); }

    /// Test for inequality with a Vector3.
    bool operator !=(const Vector3& rhs) const { return !(*this == rhs); }

    /// Test for inequality with an Vector4.
    bool operator !=(const Vector4& rhs) const { return !(*this == rhs); }

    /// Test for inequality with a Quaternion.
    bool operator !=(const Quaternion& rhs) const { return !(*this == rhs); }

    /// Test for inequality with a string.
    bool operator !=(const String& rhs) const { return !(*this == rhs); }

    /// Test for inequality with a buffer.
    bool operator !=(const PODVector<unsigned char>& rhs) const { return !(*this == rhs); }

    /// Test for inequality with a %VectorBuffer.
    bool operator !=(const VectorBuffer& rhs) const { return !(*this == rhs); }

    /// Test for inequality with a pointer.
    bool operator !=(void* rhs) const { return !(*this == rhs); }

    /// Test for inequality with a resource reference.
    bool operator !=(const ResourceRef& rhs) const { return !(*this == rhs); }

    /// Test for inequality with a resource reference list.
    bool operator !=(const ResourceRefList& rhs) const { return !(*this == rhs); }

    /// Test for inequality with a variant vector.
    bool operator !=(const VariantVector& rhs) const { return !(*this == rhs); }

    /// Test for inequality with a string vector.
    bool operator !=(const StringVector& rhs) const { return !(*this == rhs); }

    /// Test for inequality with a variant map.
    bool operator !=(const VariantMap& rhs) const { return !(*this == rhs); }

    /// Test for inequality with a rect.
    bool operator !=(const Rect& rhs) const { return !(*this == rhs); }

    /// Test for inequality with an integer rect.
    bool operator !=(const IntRect& rhs) const { return !(*this == rhs); }

    /// Test for inequality with an IntVector2.
    bool operator !=(const IntVector2& rhs) const { return !(*this == rhs); }

    /// Test for inequality with an IntVector3.
    bool operator !=(const IntVector3& rhs) const { return !(*this == rhs); }

    /// Test for inequality with a StringHash.
    bool operator !=(const StringHash& rhs) const { return !(*this == rhs); }

    /// Test for inequality with a RefCounted pointer.
    bool operator !=(RefCounted* rhs) const { return !(*this == rhs); }

    /// Test for inequality with a Matrix3.
    bool operator !=(const Matrix3& rhs) const { return !(*this == rhs); }

    /// Test for inequality with a Matrix3x4.
    bool operator !=(const Matrix3x4& rhs) const { return !(*this == rhs); }

    /// Test for inequality with a Matrix4.
    bool operator !=(const Matrix4& rhs) const { return !(*this == rhs); }

    /// Set from typename and value strings. Pointers will be set to null, and VariantBuffer or VariantMap types are not supported.
    void FromString(const String& type, const String& value);
    /// Set from typename and value strings. Pointers will be set to null, and VariantBuffer or VariantMap types are not supported.
    void FromString(const char* type, const char* value);
    /// Set from type and value string. Pointers will be set to null, and VariantBuffer or VariantMap types are not supported.
    void FromString(VariantType type, const String& value);
    /// Set from type and value string. Pointers will be set to null, and VariantBuffer or VariantMap types are not supported.
    void FromString(VariantType type, const char* value);
    /// Set buffer type from a memory area.
    void SetBuffer(const void* data, unsigned size);

    /// Return int or zero on type mismatch. Floats and doubles are converted.
    int GetInt() const
    {
        if (type_ == VAR_INT)
            return value_.int_;
        else if (type_ == VAR_FLOAT)
            return (int)value_.float_;
        else if (type_ == VAR_DOUBLE)
            return (int)*reinterpret_cast<const double*>(&value_);
        else
            return 0;
    }

    /// Return 64 bit int or zero on type mismatch. Floats and doubles are converted.
    long long GetInt64() const
    {
        if (type_ == VAR_INT64)
            return *(reinterpret_cast<const long long*>(&value_));
        else if (type_ == VAR_INT)
            return value_.int_;
        else if (type_ == VAR_FLOAT)
            return (long long)value_.float_;
        else if (type_ == VAR_DOUBLE)
            return (long long)*reinterpret_cast<const double*>(&value_);
        else
            return 0;
    }

    /// Return unsigned 64 bit int or zero on type mismatch. Floats and doubles are converted.
    unsigned long long GetUInt64() const
    {
        if (type_ == VAR_INT64)
            return *(reinterpret_cast<const unsigned long long*>(&value_));
        else if (type_ == VAR_INT)
            return static_cast<unsigned long long>(value_.int_);
        else if (type_ == VAR_FLOAT)
            return (unsigned long long)value_.float_;
        else if (type_ == VAR_DOUBLE)
            return (unsigned long long)*reinterpret_cast<const double*>(&value_);
        else
            return 0;
    }

    /// Return unsigned int or zero on type mismatch. Floats and doubles are converted.
    unsigned GetUInt() const
    {
        if (type_ == VAR_INT)
            return (unsigned)value_.int_;
        else if (type_ == VAR_FLOAT)
            return (unsigned)value_.float_;
        else if (type_ == VAR_DOUBLE)
            return (unsigned)*reinterpret_cast<const double*>(&value_);
        else
            return 0;
    }

    /// Return StringHash or zero on type mismatch.
    StringHash GetStringHash() const { return StringHash(GetUInt()); }

    /// Return bool or false on type mismatch.
    bool GetBool() const { return type_ == VAR_BOOL ? value_.bool_ : false; }

    /// Return float or zero on type mismatch. Ints and doubles are converted.
    float GetFloat() const
    {
        if (type_ == VAR_FLOAT)
            return value_.float_;
        else if (type_ == VAR_DOUBLE)
            return (float)*reinterpret_cast<const double*>(&value_);
        else if (type_ == VAR_INT)
            return (float)value_.int_;
        else
            return 0.0f;
    }

    /// Return double or zero on type mismatch. Ints and floats are converted.
    double GetDouble() const
    {
        if (type_ == VAR_DOUBLE)
            return *reinterpret_cast<const double*>(&value_);
        else if (type_ == VAR_FLOAT)
            return (double)value_.float_;
        else if (type_ == VAR_INT)
            return (double)value_.int_;
        else
            return 0.0;
    }

    /// Return Vector2 or zero on type mismatch.
    const Vector2& GetVector2() const { return type_ == VAR_VECTOR2 ? *reinterpret_cast<const Vector2*>(&value_) : Vector2::ZERO; }

    /// Return Vector3 or zero on type mismatch.
    const Vector3& GetVector3() const { return type_ == VAR_VECTOR3 ? *reinterpret_cast<const Vector3*>(&value_) : Vector3::ZERO; }

    /// Return Vector4 or zero on type mismatch.
    const Vector4& GetVector4() const { return type_ == VAR_VECTOR4 ? *reinterpret_cast<const Vector4*>(&value_) : Vector4::ZERO; }

    /// Return quaternion or identity on type mismatch.
    const Quaternion& GetQuaternion() const
    {
        return type_ == VAR_QUATERNION ? *reinterpret_cast<const Quaternion*>(&value_) : Quaternion::IDENTITY;
    }

    /// Return color or default on type mismatch. Vector4 is aliased to Color if necessary.
    const Color& GetColor() const { return (type_ == VAR_COLOR || type_ == VAR_VECTOR4) ? *reinterpret_cast<const Color*>(&value_) : Color::WHITE; }

    /// Return string or empty on type mismatch.
    const String& GetString() const { return type_ == VAR_STRING ? *reinterpret_cast<const String*>(&value_) : String::EMPTY; }

    /// Return buffer or empty on type mismatch.
    const PODVector<unsigned char>& GetBuffer() const
    {
        return type_ == VAR_BUFFER ? *reinterpret_cast<const PODVector<unsigned char>*>(&value_) : emptyBuffer;
    }

    /// Return %VectorBuffer containing the buffer or empty on type mismatch.
    VectorBuffer GetVectorBuffer() const;

    /// Return void pointer or null on type mismatch. RefCounted pointer will be converted.
    void* GetVoidPtr() const
    {
        if (type_ == VAR_VOIDPTR)
            return value_.ptr_;
        else if (type_ == VAR_PTR)
            return *reinterpret_cast<const WeakPtr<RefCounted>*>(&value_);
        else
            return 0;
    }

    /// Return a resource reference or empty on type mismatch.
    const ResourceRef& GetResourceRef() const
    {
        return type_ == VAR_RESOURCEREF ? *reinterpret_cast<const ResourceRef*>(&value_) : emptyResourceRef;
    }

    /// Return a resource reference list or empty on type mismatch.
    const ResourceRefList& GetResourceRefList() const
    {
        return type_ == VAR_RESOURCEREFLIST ? *reinterpret_cast<const ResourceRefList*>(&value_) : emptyResourceRefList;
    }

    /// Return a variant vector or empty on type mismatch.
    const VariantVector& GetVariantVector() const
    {
        return type_ == VAR_VARIANTVECTOR ? *reinterpret_cast<const VariantVector*>(&value_) : emptyVariantVector;
    }

    /// Return a string vector or empty on type mismatch.
    const StringVector& GetStringVector() const
    {
        return type_ == VAR_STRINGVECTOR ? *reinterpret_cast<const StringVector*>(&value_) : emptyStringVector;
    }

    /// Return a variant map or empty on type mismatch.
    const VariantMap& GetVariantMap() const
    {
        return type_ == VAR_VARIANTMAP ? *reinterpret_cast<const VariantMap*>(&value_) : emptyVariantMap;
    }

    /// Return a rect or empty on type mismatch.
    const Rect& GetRect() const { return type_ == VAR_RECT ? *reinterpret_cast<const Rect*>(&value_) : Rect::ZERO; }

    /// Return an integer rect or empty on type mismatch.
    const IntRect& GetIntRect() const { return type_ == VAR_INTRECT ? *reinterpret_cast<const IntRect*>(&value_) : IntRect::ZERO; }

    /// Return an IntVector2 or empty on type mismatch.
    const IntVector2& GetIntVector2() const
    {
        return type_ == VAR_INTVECTOR2 ? *reinterpret_cast<const IntVector2*>(&value_) : IntVector2::ZERO;
    }

    /// Return an IntVector3 or empty on type mismatch.
    const IntVector3& GetIntVector3() const
    {
        return type_ == VAR_INTVECTOR3 ? *reinterpret_cast<const IntVector3*>(&value_) : IntVector3::ZERO;
    }

    /// Return a RefCounted pointer or null on type mismatch. Will return null if holding a void pointer, as it can not be safely verified that the object is a RefCounted.
    RefCounted* GetPtr() const
    {
        return type_ == VAR_PTR ? *reinterpret_cast<const WeakPtr<RefCounted>*>(&value_) : (RefCounted*)0;
    }

    /// Return a Matrix3 or identity on type mismatch.
    const Matrix3& GetMatrix3() const
    {
        return type_ == VAR_MATRIX3 ? *(reinterpret_cast<const Matrix3*>(value_.ptr_)) : Matrix3::IDENTITY;
    }

    /// Return a Matrix3x4 or identity on type mismatch.
    const Matrix3x4& GetMatrix3x4() const
    {
        return type_ == VAR_MATRIX3X4 ? *(reinterpret_cast<const Matrix3x4*>(value_.ptr_)) : Matrix3x4::IDENTITY;
    }

    /// Return a Matrix4 or identity on type mismatch.
    const Matrix4& GetMatrix4() const
    {
        return type_ == VAR_MATRIX4 ? *(reinterpret_cast<const Matrix4*>(value_.ptr_)) : Matrix4::IDENTITY;
    }

    /// Return value's type.
    VariantType GetType() const { return type_; }

    /// Return value's type name.
    String GetTypeName() const;
    /// Convert value to string. Pointers are returned as null, and VariantBuffer or VariantMap are not supported and return empty.
    String ToString() const;
    /// Return true when the variant value is considered zero according to its actual type.
    bool IsZero() const;

    /// Return true when the variant is empty (i.e. not initialized yet).
    bool IsEmpty() const { return type_ == VAR_NONE; }

    /// Return the value, template version.
    template <class T> T Get() const;

    /// Return a pointer to a modifiable buffer or null on type mismatch.
    PODVector<unsigned char>* GetBufferPtr()
    {
        return type_ == VAR_BUFFER ? reinterpret_cast<PODVector<unsigned char>*>(&value_) : 0;
    }

    /// Return a pointer to a modifiable variant vector or null on type mismatch.
    VariantVector* GetVariantVectorPtr() { return type_ == VAR_VARIANTVECTOR ? reinterpret_cast<VariantVector*>(&value_) : 0; }

    /// Return a pointer to a modifiable string vector or null on type mismatch.
    StringVector* GetStringVectorPtr() { return type_ == VAR_STRINGVECTOR ? reinterpret_cast<StringVector*>(&value_) : 0; }

    /// Return a pointer to a modifiable variant map or null on type mismatch.
    VariantMap* GetVariantMapPtr() { return type_ == VAR_VARIANTMAP ? reinterpret_cast<VariantMap*>(&value_) : 0; }

    /// Return name for variant type.
    static String GetTypeName(VariantType type);
    /// Return variant type from type name.
    static VariantType GetTypeFromName(const String& typeName);
    /// Return variant type from type name.
    static VariantType GetTypeFromName(const char* typeName);

    /// Empty variant.
    static const Variant EMPTY;
    /// Empty buffer.
    static const PODVector<unsigned char> emptyBuffer;
    /// Empty resource reference.
    static const ResourceRef emptyResourceRef;
    /// Empty resource reference list.
    static const ResourceRefList emptyResourceRefList;
    /// Empty variant map.
    static const VariantMap emptyVariantMap;
    /// Empty variant vector.
    static const VariantVector emptyVariantVector;
    /// Empty string vector.
    static const StringVector emptyStringVector;

private:
    /// Set new type and allocate/deallocate memory as necessary.
    void SetType(VariantType newType);

    /// Variant type.
    VariantType type_;
    /// Variant value.
    VariantValue value_;
};

/// Return variant type from type.
template <typename T> VariantType GetVariantType();

// Return variant type from concrete types
template <> inline VariantType GetVariantType<int>() { return VAR_INT; }

template <> inline VariantType GetVariantType<unsigned>() { return VAR_INT; }

template <> inline VariantType GetVariantType<long long>() { return VAR_INT64; }

template <> inline VariantType GetVariantType<unsigned long long>() { return VAR_INT64; }

template <> inline VariantType GetVariantType<bool>() { return VAR_BOOL; }

template <> inline VariantType GetVariantType<float>() { return VAR_FLOAT; }

template <> inline VariantType GetVariantType<double>() { return VAR_DOUBLE; }

template <> inline VariantType GetVariantType<Vector2>() { return VAR_VECTOR2; }

template <> inline VariantType GetVariantType<Vector3>() { return VAR_VECTOR3; }

template <> inline VariantType GetVariantType<Vector4>() { return VAR_VECTOR4; }

template <> inline VariantType GetVariantType<Quaternion>() { return VAR_QUATERNION; }

template <> inline VariantType GetVariantType<Color>() { return VAR_COLOR; }

template <> inline VariantType GetVariantType<String>() { return VAR_STRING; }

template <> inline VariantType GetVariantType<StringHash>() { return VAR_INT; }

template <> inline VariantType GetVariantType<PODVector<unsigned char> >() { return VAR_BUFFER; }

template <> inline VariantType GetVariantType<ResourceRef>() { return VAR_RESOURCEREF; }

template <> inline VariantType GetVariantType<ResourceRefList>() { return VAR_RESOURCEREFLIST; }

template <> inline VariantType GetVariantType<VariantVector>() { return VAR_VARIANTVECTOR; }

template <> inline VariantType GetVariantType<StringVector>() { return VAR_STRINGVECTOR; }

template <> inline VariantType GetVariantType<VariantMap>() { return VAR_VARIANTMAP; }

template <> inline VariantType GetVariantType<Rect>() { return VAR_RECT; }

template <> inline VariantType GetVariantType<IntRect>() { return VAR_INTRECT; }

template <> inline VariantType GetVariantType<IntVector2>() { return VAR_INTVECTOR2; }

template <> inline VariantType GetVariantType<IntVector3>() { return VAR_INTVECTOR3; }

template <> inline VariantType GetVariantType<Matrix3>() { return VAR_MATRIX3; }

template <> inline VariantType GetVariantType<Matrix3x4>() { return VAR_MATRIX3X4; }

template <> inline VariantType GetVariantType<Matrix4>() { return VAR_MATRIX4; }

// Specializations of Variant::Get<T>
template <> ATOMIC_API int Variant::Get<int>() const;

template <> ATOMIC_API unsigned Variant::Get<unsigned>() const;

template <> ATOMIC_API long long Variant::Get<long long>() const;

template <> ATOMIC_API unsigned long long Variant::Get<unsigned long long>() const;

template <> ATOMIC_API StringHash Variant::Get<StringHash>() const;

template <> ATOMIC_API bool Variant::Get<bool>() const;

template <> ATOMIC_API float Variant::Get<float>() const;

template <> ATOMIC_API double Variant::Get<double>() const;

template <> ATOMIC_API const Vector2& Variant::Get<const Vector2&>() const;

template <> ATOMIC_API const Vector3& Variant::Get<const Vector3&>() const;

template <> ATOMIC_API const Vector4& Variant::Get<const Vector4&>() const;

template <> ATOMIC_API const Quaternion& Variant::Get<const Quaternion&>() const;

template <> ATOMIC_API const Color& Variant::Get<const Color&>() const;

template <> ATOMIC_API const String& Variant::Get<const String&>() const;

template <> ATOMIC_API const Rect& Variant::Get<const Rect&>() const;

template <> ATOMIC_API const IntRect& Variant::Get<const IntRect&>() const;

template <> ATOMIC_API const IntVector2& Variant::Get<const IntVector2&>() const;

template <> ATOMIC_API const IntVector3& Variant::Get<const IntVector3&>() const;

template <> ATOMIC_API const PODVector<unsigned char>& Variant::Get<const PODVector<unsigned char>&>() const;

template <> ATOMIC_API void* Variant::Get<void*>() const;

template <> ATOMIC_API RefCounted* Variant::Get<RefCounted*>() const;

template <> ATOMIC_API const Matrix3& Variant::Get<const Matrix3&>() const;

template <> ATOMIC_API const Matrix3x4& Variant::Get<const Matrix3x4&>() const;

template <> ATOMIC_API const Matrix4& Variant::Get<const Matrix4&>() const;

template <> ATOMIC_API ResourceRef Variant::Get<ResourceRef>() const;

template <> ATOMIC_API ResourceRefList Variant::Get<ResourceRefList>() const;

template <> ATOMIC_API VariantVector Variant::Get<VariantVector>() const;

template <> ATOMIC_API StringVector Variant::Get<StringVector>() const;

template <> ATOMIC_API VariantMap Variant::Get<VariantMap>() const;

template <> ATOMIC_API Vector2 Variant::Get<Vector2>() const;

template <> ATOMIC_API Vector3 Variant::Get<Vector3>() const;

template <> ATOMIC_API Vector4 Variant::Get<Vector4>() const;

template <> ATOMIC_API Quaternion Variant::Get<Quaternion>() const;

template <> ATOMIC_API Color Variant::Get<Color>() const;

template <> ATOMIC_API String Variant::Get<String>() const;

template <> ATOMIC_API Rect Variant::Get<Rect>() const;

template <> ATOMIC_API IntRect Variant::Get<IntRect>() const;

template <> ATOMIC_API IntVector2 Variant::Get<IntVector2>() const;

template <> ATOMIC_API IntVector3 Variant::Get<IntVector3>() const;

template <> ATOMIC_API PODVector<unsigned char> Variant::Get<PODVector<unsigned char> >() const;

template <> ATOMIC_API Matrix3 Variant::Get<Matrix3>() const;

template <> ATOMIC_API Matrix3x4 Variant::Get<Matrix3x4>() const;

template <> ATOMIC_API Matrix4 Variant::Get<Matrix4>() const;

}
