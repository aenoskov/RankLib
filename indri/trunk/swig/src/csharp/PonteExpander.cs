/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.29
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

namespace Indri {

using System;
using System.Runtime.InteropServices;

public class PonteExpander : QueryExpander {
  private HandleRef swigCPtr;

  internal PonteExpander(IntPtr cPtr, bool cMemoryOwn) : base(indriPINVOKE.PonteExpanderUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(PonteExpander obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~PonteExpander() {
    Dispose();
  }

  public override void Dispose() {
    if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
      swigCMemOwn = false;
      indriPINVOKE.delete_PonteExpander(swigCPtr);
    }
    swigCPtr = new HandleRef(null, IntPtr.Zero);
    GC.SuppressFinalize(this);
    base.Dispose();
  }

  public PonteExpander(QueryEnvironment env, Parameters param) : this(indriPINVOKE.new_PonteExpander(QueryEnvironment.getCPtr(env), Parameters.getCPtr(param)), true) {
    if (indriPINVOKE.SWIGPendingException.Pending) throw indriPINVOKE.SWIGPendingException.Retrieve();
  }

  public override string expand(string originalQuery, ScoredExtentResultVector results) {
    string ret = indriPINVOKE.PonteExpander_expand(swigCPtr, originalQuery, ScoredExtentResultVector.getCPtr(results));
    if (indriPINVOKE.SWIGPendingException.Pending) throw indriPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}
