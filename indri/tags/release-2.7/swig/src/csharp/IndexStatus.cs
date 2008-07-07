/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.31
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

namespace Indri {

using System;
using System.Runtime.InteropServices;

public class IndexStatus : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal IndexStatus(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(IndexStatus obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~IndexStatus() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
        swigCMemOwn = false;
        indri_csharpPINVOKE.delete_IndexStatus(swigCPtr);
      }
      swigCPtr = new HandleRef(null, IntPtr.Zero);
      GC.SuppressFinalize(this);
    }
  }

  public virtual void status(int code, string documentPath, string error, int documentsIndexed, int documentsSeen) {
    indri_csharpPINVOKE.IndexStatus_status(swigCPtr, code, documentPath, error, documentsIndexed, documentsSeen);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public IndexStatus() : this(indri_csharpPINVOKE.new_IndexStatus(), true) {
    SwigDirectorConnect();
  }

  private void SwigDirectorConnect() {
    if (SwigDerivedClassHasMethod("status", swigMethodTypes0))
      swigDelegate0 = new SwigDelegateIndexStatus_0(SwigDirectorstatus);
    indri_csharpPINVOKE.IndexStatus_director_connect(swigCPtr, swigDelegate0);
  }

  private bool SwigDerivedClassHasMethod(string methodName, Type[] methodTypes) {
    System.Reflection.MethodInfo methodInfo = this.GetType().GetMethod(methodName, System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance, null, methodTypes, null);
    bool hasDerivedMethod = methodInfo.DeclaringType.IsSubclassOf(typeof(IndexStatus));
    return hasDerivedMethod;
  }

  private void SwigDirectorstatus(int code, string documentPath, string error, int documentsIndexed, int documentsSeen) {
    status(code, documentPath, error, documentsIndexed, documentsSeen);
  }

  public delegate void SwigDelegateIndexStatus_0(int code, string documentPath, string error, int documentsIndexed, int documentsSeen);

  private SwigDelegateIndexStatus_0 swigDelegate0;

  private static Type[] swigMethodTypes0 = new Type[] { typeof(int), typeof(string), typeof(string), typeof(int), typeof(int) };
  public enum action_code {
    FileOpen,
    FileSkip,
    FileError,
    FileClose,
    DocumentCount
  }

}

}