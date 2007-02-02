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

public class MResults : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal MResults(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(MResults obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~MResults() {
    Dispose();
  }

  public virtual void Dispose() {
    if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
      swigCMemOwn = false;
      indri_csharpPINVOKE.delete_MResults(swigCPtr);
    }
    swigCPtr = new HandleRef(null, IntPtr.Zero);
    GC.SuppressFinalize(this);
  }

  public MResults() : this(indri_csharpPINVOKE.new_MResults__SWIG_0(), true) {
  }

  public MResults(MResults arg0) : this(indri_csharpPINVOKE.new_MResults__SWIG_1(MResults.getCPtr(arg0)), true) {
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public uint size() {
    uint ret = indri_csharpPINVOKE.MResults_size(swigCPtr);
    return ret;
  }

  public bool empty() {
    bool ret = indri_csharpPINVOKE.MResults_empty(swigCPtr);
    return ret;
  }

  public void clear() {
    indri_csharpPINVOKE.MResults_clear(swigCPtr);
  }

  public ScoredExtentResultVector get(string key) {
    ScoredExtentResultVector ret = new ScoredExtentResultVector(indri_csharpPINVOKE.MResults_get(swigCPtr, key), false);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void set(string key, ScoredExtentResultVector x) {
    indri_csharpPINVOKE.MResults_set(swigCPtr, key, ScoredExtentResultVector.getCPtr(x));
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void del(string key) {
    indri_csharpPINVOKE.MResults_del(swigCPtr, key);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public bool has_key(string key) {
    bool ret = indri_csharpPINVOKE.MResults_has_key(swigCPtr, key);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}