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

public class ConflationPattern : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal ConflationPattern(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(ConflationPattern obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~ConflationPattern() {
    Dispose();
  }

  public virtual void Dispose() {
    if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
      swigCMemOwn = false;
      indri_csharpPINVOKE.delete_ConflationPattern(swigCPtr);
    }
    swigCPtr = new HandleRef(null, IntPtr.Zero);
    GC.SuppressFinalize(this);
  }

  public string tag_name {
    set {
      indri_csharpPINVOKE.ConflationPattern_tag_name_set(swigCPtr, value);
    } 
    get {
      string ret = indri_csharpPINVOKE.ConflationPattern_tag_name_get(swigCPtr);
      return ret;
    } 
  }

  public string attribute_name {
    set {
      indri_csharpPINVOKE.ConflationPattern_attribute_name_set(swigCPtr, value);
    } 
    get {
      string ret = indri_csharpPINVOKE.ConflationPattern_attribute_name_get(swigCPtr);
      return ret;
    } 
  }

  public string value {
    set {
      indri_csharpPINVOKE.ConflationPattern_value_set(swigCPtr, value);
    } 
    get {
      string ret = indri_csharpPINVOKE.ConflationPattern_value_get(swigCPtr);
      return ret;
    } 
  }

  public ConflationPattern() : this(indri_csharpPINVOKE.new_ConflationPattern(), true) {
  }

}

}
