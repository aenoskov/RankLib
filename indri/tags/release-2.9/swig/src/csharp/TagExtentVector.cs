/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.36
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

namespace Indri {

using System;
using System.Runtime.InteropServices;

public class TagExtentVector : IDisposable, System.Collections.IEnumerable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal TagExtentVector(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(TagExtentVector obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~TagExtentVector() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
        swigCMemOwn = false;
        indri_csharpPINVOKE.delete_TagExtentVector(swigCPtr);
      }
      swigCPtr = new HandleRef(null, IntPtr.Zero);
      GC.SuppressFinalize(this);
    }
  }

  public TagExtentVector(System.Collections.ICollection c) : this() {
    if (c == null)
      throw new ArgumentNullException("c");
    foreach (TagExtent element in c) {
      this.Add(element);
    }
  }

  public bool IsFixedSize {
    get {
      return false;
    }
  }

  public bool IsReadOnly {
    get {
      return false;
    }
  }

  public TagExtent this[int index]  {
    get {
      return getitem(index);
    }
    set {
      setitem(index, value);
    }
  }

  public int Capacity {
    get {
      return (int)capacity();
    }
    set {
      if (value < size())
        throw new ArgumentOutOfRangeException("Capacity");
      reserve((uint)value);
    }
  }

  public int Count {
    get {
      return (int)size();
    }
  }

  public bool IsSynchronized {
    get {
      return false;
    }
  }

  public void CopyTo(System.Array array) {
    CopyTo(0, array, 0, this.Count);
  }

  public void CopyTo(System.Array array, int arrayIndex) {
    CopyTo(0, array, arrayIndex, this.Count);
  }

  public void CopyTo(int index, System.Array array, int arrayIndex, int count) {
    if (array == null)
      throw new ArgumentNullException("array");
    if (index < 0)
      throw new ArgumentOutOfRangeException("index", "Value is less than zero");
    if (arrayIndex < 0)
      throw new ArgumentOutOfRangeException("arrayIndex", "Value is less than zero");
    if (count < 0)
      throw new ArgumentOutOfRangeException("count", "Value is less than zero");
    if (array.Rank > 1)
      throw new ArgumentException("Multi dimensional array.");
    if (index+count > this.Count || arrayIndex+count > array.Length)
      throw new ArgumentException("Number of elements to copy is too large.");
    for (int i=0; i<count; i++)
      array.SetValue(getitemcopy(index+i), arrayIndex+i);
  }

  // Type-safe version of IEnumerable.GetEnumerator
  System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() {
    return new TagExtentVectorEnumerator(this);
  }

  public TagExtentVectorEnumerator GetEnumerator() {
    return new TagExtentVectorEnumerator(this);
  }

  // Type-safe enumerator
  /// Note that the IEnumerator documentation requires an InvalidOperationException to be thrown
  /// whenever the collection is modified. This has been done for changes in the size of the
  /// collection but not when one of the elements of the collection is modified as it is a bit
  /// tricky to detect unmanaged code that modifies the collection under our feet.
  public sealed class TagExtentVectorEnumerator : System.Collections.IEnumerator {
    private TagExtentVector collectionRef;
    private int currentIndex;
    private object currentObject;
    private int currentSize;

    public TagExtentVectorEnumerator(TagExtentVector collection) {
      collectionRef = collection;
      currentIndex = -1;
      currentObject = null;
      currentSize = collectionRef.Count;
    }

    // Type-safe iterator Current
    public TagExtent Current {
      get {
        if (currentIndex == -1)
          throw new InvalidOperationException("Enumeration not started.");
        if (currentIndex > currentSize - 1)
          throw new InvalidOperationException("Enumeration finished.");
        if (currentObject == null)
          throw new InvalidOperationException("Collection modified.");
        return (TagExtent)currentObject;
      }
    }

    // Type-unsafe IEnumerator.Current
    object System.Collections.IEnumerator.Current {
      get {
        return Current;
      }
    }

    public bool MoveNext() {
      int size = collectionRef.Count;
      bool moveOkay = (currentIndex+1 < size) && (size == currentSize);
      if (moveOkay) {
        currentIndex++;
        currentObject = collectionRef[currentIndex];
      } else {
        currentObject = null;
      }
      return moveOkay;
    }

    public void Reset() {
      currentIndex = -1;
      currentObject = null;
      if (collectionRef.Count != currentSize) {
        throw new InvalidOperationException("Collection modified.");
      }
    }
  }

  public void Clear() {
    indri_csharpPINVOKE.TagExtentVector_Clear(swigCPtr);
  }

  public void Add(TagExtent value) {
    indri_csharpPINVOKE.TagExtentVector_Add(swigCPtr, TagExtent.getCPtr(value));
  }

  private uint size() {
    uint ret = indri_csharpPINVOKE.TagExtentVector_size(swigCPtr);
    return ret;
  }

  private uint capacity() {
    uint ret = indri_csharpPINVOKE.TagExtentVector_capacity(swigCPtr);
    return ret;
  }

  private void reserve(uint n) {
    indri_csharpPINVOKE.TagExtentVector_reserve(swigCPtr, n);
  }

  public TagExtentVector() : this(indri_csharpPINVOKE.new_TagExtentVector__SWIG_0(), true) {
  }

  public TagExtentVector(int capacity) : this(indri_csharpPINVOKE.new_TagExtentVector__SWIG_1(capacity), true) {
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  private TagExtent getitemcopy(int index) {
    IntPtr cPtr = indri_csharpPINVOKE.TagExtentVector_getitemcopy(swigCPtr, index);
    TagExtent ret = (cPtr == IntPtr.Zero) ? null : new TagExtent(cPtr, false);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private TagExtent getitem(int index) {
    IntPtr cPtr = indri_csharpPINVOKE.TagExtentVector_getitem(swigCPtr, index);
    TagExtent ret = (cPtr == IntPtr.Zero) ? null : new TagExtent(cPtr, false);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private void setitem(int index, TagExtent value) {
    indri_csharpPINVOKE.TagExtentVector_setitem(swigCPtr, index, TagExtent.getCPtr(value));
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void AddRange(TagExtentVector values) {
    indri_csharpPINVOKE.TagExtentVector_AddRange(swigCPtr, TagExtentVector.getCPtr(values));
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public TagExtentVector GetRange(int index, int count) {
    IntPtr cPtr = indri_csharpPINVOKE.TagExtentVector_GetRange(swigCPtr, index, count);
    TagExtentVector ret = (cPtr == IntPtr.Zero) ? null : new TagExtentVector(cPtr, true);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Insert(int index, TagExtent value) {
    indri_csharpPINVOKE.TagExtentVector_Insert(swigCPtr, index, TagExtent.getCPtr(value));
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void InsertRange(int index, TagExtentVector values) {
    indri_csharpPINVOKE.TagExtentVector_InsertRange(swigCPtr, index, TagExtentVector.getCPtr(values));
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveAt(int index) {
    indri_csharpPINVOKE.TagExtentVector_RemoveAt(swigCPtr, index);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveRange(int index, int count) {
    indri_csharpPINVOKE.TagExtentVector_RemoveRange(swigCPtr, index, count);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public static TagExtentVector Repeat(TagExtent value, int count) {
    IntPtr cPtr = indri_csharpPINVOKE.TagExtentVector_Repeat(TagExtent.getCPtr(value), count);
    TagExtentVector ret = (cPtr == IntPtr.Zero) ? null : new TagExtentVector(cPtr, true);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Reverse() {
    indri_csharpPINVOKE.TagExtentVector_Reverse__SWIG_0(swigCPtr);
  }

  public void Reverse(int index, int count) {
    indri_csharpPINVOKE.TagExtentVector_Reverse__SWIG_1(swigCPtr, index, count);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetRange(int index, TagExtentVector values) {
    indri_csharpPINVOKE.TagExtentVector_SetRange(swigCPtr, index, TagExtentVector.getCPtr(values));
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

}

}
