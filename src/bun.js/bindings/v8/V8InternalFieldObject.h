#pragma once

#include "V8ObjectTemplate.h"

namespace v8 {

class InternalFieldObject : public JSC::JSDestructibleObject {
public:
    using Base = JSC::JSDestructibleObject;

    DECLARE_INFO;

    template<typename, JSC::SubspaceAccess mode>
    static JSC::GCClient::IsoSubspace* subspaceFor(JSC::VM& vm)
    {
        if constexpr (mode == JSC::SubspaceAccess::Concurrently)
            return nullptr;
        return WebCore::subspaceForImpl<InternalFieldObject, WebCore::UseCustomHeapCellType::No>(
            vm,
            [](auto& spaces) { return spaces.m_clientSubspaceForInternalFieldObject.get(); },
            [](auto& spaces, auto&& space) { spaces.m_clientSubspaceForInternalFieldObject = std::forward<decltype(space)>(space); },
            [](auto& spaces) { return spaces.m_subspaceForInternalFieldObject.get(); },
            [](auto& spaces, auto&& space) { spaces.m_subspaceForInternalFieldObject = std::forward<decltype(space)>(space); });
    }

    using FieldContainer = WTF::Vector<JSC::JSValue, 2>;

    FieldContainer* internalFields() { return &fields; }
    static InternalFieldObject* create(JSC::VM& vm, JSC::Structure* structure, Local<ObjectTemplate> objectTemplate);

protected:
    InternalFieldObject(JSC::VM& vm, JSC::Structure* structure, int internalFieldCount)
        : Base(vm, structure)
        , fields(internalFieldCount)
    {
    }

private:
    // TODO(@190n) use template with fixed size array for small counts
    FieldContainer fields;
};

}
