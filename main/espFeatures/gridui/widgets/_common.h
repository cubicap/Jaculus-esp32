#pragma once

#include <jac/machine/values.h>

namespace gridui_jac {

using qjsGetter = JSValue(*)(JSContext* ctx_, JSValueConst thisVal);
using qjsSetter = JSValue(*)(JSContext* ctx_, JSValueConst thisVal, JSValueConst val);

static inline void defineWidgetProperty(jac::ContextRef ctx, jac::Object& target,
    const char *name, const char *setterName,
    qjsGetter getter, qjsSetter setter) {

    auto jsGetter = JS_NewCFunction2(ctx.get(), reinterpret_cast<JSCFunction*>(reinterpret_cast<void*>(getter)), name, 0, JS_CFUNC_getter, 0);
    auto jsSetter = JS_NewCFunction2(ctx.get(), reinterpret_cast<JSCFunction*>(reinterpret_cast<void*>(setter)), setterName, 1, JS_CFUNC_setter, 0);

    JS_DefinePropertyGetSet(
        ctx.get(), target.getVal(),
        jac::Atom::create(ctx, name).get(), 
        jsGetter, jsSetter, 0
    );
}

static inline void defineWidgetPropertyReadOnly(jac::ContextRef ctx, jac::Object& target, const char *name, qjsGetter getter) {
    auto jsGetter = JS_NewCFunction2(ctx.get(), reinterpret_cast<JSCFunction*>(reinterpret_cast<void*>(getter)), name, 0, JS_CFUNC_getter, 0);
    JS_DefineProperty(ctx, target.getVal(), jac::Atom::create(ctx, name).get(), JS_UNDEFINED, jsGetter, JS_UNDEFINED, JS_PROP_HAS_GET | JS_PROP_HAS_ENUMERABLE);
    JS_FreeValue(ctx, jsGetter);
}

};
