/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 * vim: set ts=8 sts=4 et sw=4 tw=99:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef jsscriptinlines_h
#define jsscriptinlines_h

#include "jsscript.h"

#include "jit/AsmJSLink.h"
#include "jit/BaselineJIT.h"
#include "vm/ScopeObject.h"

#include "jscompartmentinlines.h"

#include "vm/Shape-inl.h"

namespace js {

inline
Bindings::Bindings()
    : callObjShape_(NULL), bindingArrayAndFlag_(TEMPORARY_STORAGE_BIT), numArgs_(0), numVars_(0)
{}

inline
AliasedFormalIter::AliasedFormalIter(JSScript *script)
  : begin_(script->bindings.bindingArray()),
    p_(begin_),
    end_(begin_ + (script->funHasAnyAliasedFormal ? script->bindings.numArgs() : 0)),
    slot_(CallObject::RESERVED_SLOTS)
{
    settle();
}

inline void
ScriptCounts::destroy(FreeOp *fop)
{
    fop->free_(pcCountsVector);
    fop->delete_(ionCounts);
}

void
SetFrameArgumentsObject(JSContext *cx, AbstractFramePtr frame,
                        HandleScript script, JSObject *argsobj);

} // namespace js

inline void
JSScript::setFunction(JSFunction *fun)
{
    JS_ASSERT(fun->isTenured());
    function_ = fun;
}

inline JSFunction *
JSScript::getFunction(size_t index)
{
    JSFunction *fun = &getObject(index)->as<JSFunction>();
#ifdef DEBUG
    JS_ASSERT_IF(fun->isNative(), IsAsmJSModuleNative(fun->native()));
#endif
    return fun;
}

inline JSFunction *
JSScript::getCallerFunction()
{
    JS_ASSERT(savedCallerFun);
    return getFunction(0);
}

inline JSFunction *
JSScript::functionOrCallerFunction()
{
    if (function())
        return function();
    if (savedCallerFun)
        return getCallerFunction();
    return NULL;
}

inline js::RegExpObject *
JSScript::getRegExp(size_t index)
{
    js::ObjectArray *arr = regexps();
    JS_ASSERT(uint32_t(index) < arr->length);
    JSObject *obj = arr->vector[index];
    JS_ASSERT(obj->is<js::RegExpObject>());
    return (js::RegExpObject *) obj;
}

inline js::GlobalObject &
JSScript::global() const
{
    /*
     * A JSScript always marks its compartment's global (via bindings) so we
     * can assert that maybeGlobal is non-null here.
     */
    return *compartment()->maybeGlobal();
}

inline void
JSScript::writeBarrierPre(JSScript *script)
{
#ifdef JSGC_INCREMENTAL
    if (!script || !script->runtimeFromAnyThread()->needsBarrier())
        return;

    JS::Zone *zone = script->zone();
    if (zone->needsBarrier()) {
        JS_ASSERT(!zone->runtimeFromMainThread()->isHeapMajorCollecting());
        JSScript *tmp = script;
        MarkScriptUnbarriered(zone->barrierTracer(), &tmp, "write barrier");
        JS_ASSERT(tmp == script);
    }
#endif
}

/* static */ inline void
js::LazyScript::writeBarrierPre(js::LazyScript *lazy)
{
#ifdef JSGC_INCREMENTAL
    if (!lazy || !lazy->runtimeFromAnyThread()->needsBarrier())
        return;

    JS::Zone *zone = lazy->zone();
    if (zone->needsBarrier()) {
        JS_ASSERT(!zone->runtimeFromMainThread()->isHeapMajorCollecting());
        js::LazyScript *tmp = lazy;
        MarkLazyScriptUnbarriered(zone->barrierTracer(), &tmp, "write barrier");
        JS_ASSERT(tmp == lazy);
    }
#endif
}

inline JSPrincipals *
JSScript::principals()
{
    return compartment()->principals;
}

inline JSFunction *
JSScript::originalFunction() const {
    if (!isCallsiteClone)
        return NULL;
    return &enclosingScopeOrOriginalFunction_->as<JSFunction>();
}

inline void
JSScript::setOriginalFunctionObject(JSObject *fun) {
    JS_ASSERT(isCallsiteClone);
    JS_ASSERT(fun->is<JSFunction>());
    enclosingScopeOrOriginalFunction_ = fun;
}

inline void
JSScript::setIonScript(js::ion::IonScript *ionScript) {
    if (hasIonScript())
        js::ion::IonScript::writeBarrierPre(tenuredZone(), ion);
    ion = ionScript;
    updateBaselineOrIonRaw();
}

inline void
JSScript::setParallelIonScript(js::ion::IonScript *ionScript) {
    if (hasParallelIonScript())
        js::ion::IonScript::writeBarrierPre(tenuredZone(), parallelIon);
    parallelIon = ionScript;
}

inline void
JSScript::setBaselineScript(js::ion::BaselineScript *baselineScript) {
#ifdef JS_ION
    if (hasBaselineScript())
        js::ion::BaselineScript::writeBarrierPre(tenuredZone(), baseline);
#endif
    baseline = baselineScript;
    updateBaselineOrIonRaw();
}

#endif /* jsscriptinlines_h */
