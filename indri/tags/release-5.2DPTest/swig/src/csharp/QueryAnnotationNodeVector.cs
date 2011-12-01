/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.0
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

namespace Indri {

using System;
using System.Runtime.InteropServices;

public class QueryAnnotationNodeVector : IDisposable, System.Collections.IEnumerable
#if !SWIG_DOTNET_1
    , System.Collections.Generic.IList<QueryAnnotationNode>
#endif
 {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal QueryAnnotationNodeVector(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(QueryAnnotationNodeVector obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~QueryAnnotationNodeVector() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          indri_csharpPINVOKE.delete_QueryAnnotationNodeVector(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
    }
  }

  public QueryAnnotationNodeVector(System.Collections.ICollection c) : this() {
    if (c == null)
      throw new ArgumentNullException("c");
    foreach (QueryAnnotationNode element in c) {
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

  public QueryAnnotationNode this[int index]  {
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

#if SWIG_DOTNET_1
  public void CopyTo(System.Array array)
#else
  public void CopyTo(QueryAnnotationNode[] array)
#endif
  {
    CopyTo(0, array, 0, this.Count);
  }

#if SWIG_DOTNET_1
  public void CopyTo(System.Array array, int arrayIndex)
#else
  public void CopyTo(QueryAnnotationNode[] array, int arrayIndex)
#endif
  {
    CopyTo(0, array, arrayIndex, this.Count);
  }

#if SWIG_DOTNET_1
  public void CopyTo(int index, System.Array array, int arrayIndex, int count)
#else
  public void CopyTo(int index, QueryAnnotationNode[] array, int arrayIndex, int count)
#endif
  {
    if (array == null)
      throw new ArgumentNullException("array");
    if (index < 0)
      throw new ArgumentOutOfRangeException("index", "Value is less than zero");
    if (arrayIndex < 0)
      throw new ArgumentOutOfRangeException("arrayIndex", "Value is less than zero");
    if (count < 0)
      throw new ArgumentOutOfRangeException("count", "Value is less than zero");
    if (array.Rank > 1)
      throw new ArgumentException("Multi dimensional array.", "array");
    if (index+count > this.Count || arrayIndex+count > array.Length)
      throw new ArgumentException("Number of elements to copy is too large.");
    for (int i=0; i<count; i++)
      array.SetValue(getitemcopy(index+i), arrayIndex+i);
  }

#if !SWIG_DOTNET_1
  System.Collections.Generic.IEnumerator<QueryAnnotationNode> System.Collections.Generic.IEnumerable<QueryAnnotationNode>.GetEnumerator() {
    return new QueryAnnotationNodeVectorEnumerator(this);
  }
#endif

  System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() {
    return new QueryAnnotationNodeVectorEnumerator(this);
  }

  public QueryAnnotationNodeVectorEnumerator GetEnumerator() {
    return new QueryAnnotationNodeVectorEnumerator(this);
  }

  // Type-safe enumerator
  /// Note that the IEnumerator documentation requires an InvalidOperationException to be thrown
  /// whenever the collection is modified. This has been done for changes in the size of the
  /// collection but not when one of the elements of the collection is modified as it is a bit
  /// tricky to detect unmanaged code that modifies the collection under our feet.
  public sealed class QueryAnnotationNodeVectorEnumerator : System.Collections.IEnumerator
#if !SWIG_DOTNET_1
    , System.Collections.Generic.IEnumerator<QueryAnnotationNode>
#endif
  {
    private QueryAnnotationNodeVector collectionRef;
    private int currentIndex;
    private object currentObject;
    private int currentSize;

    public QueryAnnotationNodeVectorEnumerator(QueryAnnotationNodeVector collection) {
      collectionRef = collection;
      currentIndex = -1;
      currentObject = null;
      currentSize = collectionRef.Count;
    }

    // Type-safe iterator Current
    public QueryAnnotationNode Current {
      get {
        if (currentIndex == -1)
          throw new InvalidOperationException("Enumeration not started.");
        if (currentIndex > currentSize - 1)
          throw new InvalidOperationException("Enumeration finished.");
        if (currentObject == null)
          throw new InvalidOperationException("Collection modified.");
        return (QueryAnnotationNode)currentObject;
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

#if !SWIG_DOTNET_1
    public void Dispose() {
        currentIndex = -1;
        currentObject = null;
    }
#endif
  }

  public void Clear() {
    indri_csharpPINVOKE.QueryAnnotationNodeVector_Clear(swigCPtr);
  }

  public void Add(QueryAnnotationNode x) {
    indri_csharpPINVOKE.QueryAnnotationNodeVector_Add(swigCPtr, QueryAnnotationNode.getCPtr(x));
  }

  private uint size() {
    uint ret = indri_csharpPINVOKE.QueryAnnotationNodeVector_size(swigCPtr);
    return ret;
  }

  private uint capacity() {
    uint ret = indri_csharpPINVOKE.QueryAnnotationNodeVector_capacity(swigCPtr);
    return ret;
  }

  private void reserve(uint n) {
    indri_csharpPINVOKE.QueryAnnotationNodeVector_reserve(swigCPtr, n);
  }

  public QueryAnnotationNodeVector() : this(indri_csharpPINVOKE.new_QueryAnnotationNodeVector__SWIG_0(), true) {
  }

  public QueryAnnotationNodeVector(QueryAnnotationNodeVector other) : this(indri_csharpPINVOKE.new_QueryAnnotationNodeVector__SWIG_1(QueryAnnotationNodeVector.getCPtr(other)), true) {
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public QueryAnnotationNodeVector(int capacity) : this(indri_csharpPINVOKE.new_QueryAnnotationNodeVector__SWIG_2(capacity), true) {
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  private QueryAnnotationNode getitemcopy(int index) {
    IntPtr cPtr = indri_csharpPINVOKE.QueryAnnotationNodeVector_getitemcopy(swigCPtr, index);
    QueryAnnotationNode ret = (cPtr == IntPtr.Zero) ? null : new QueryAnnotationNode(cPtr, false);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private QueryAnnotationNode getitem(int index) {
    IntPtr cPtr = indri_csharpPINVOKE.QueryAnnotationNodeVector_getitem(swigCPtr, index);
    QueryAnnotationNode ret = (cPtr == IntPtr.Zero) ? null : new QueryAnnotationNode(cPtr, false);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private void setitem(int index, QueryAnnotationNode val) {
    indri_csharpPINVOKE.QueryAnnotationNodeVector_setitem(swigCPtr, index, QueryAnnotationNode.getCPtr(val));
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void AddRange(QueryAnnotationNodeVector values) {
    indri_csharpPINVOKE.QueryAnnotationNodeVector_AddRange(swigCPtr, QueryAnnotationNodeVector.getCPtr(values));
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public QueryAnnotationNodeVector GetRange(int index, int count) {
    IntPtr cPtr = indri_csharpPINVOKE.QueryAnnotationNodeVector_GetRange(swigCPtr, index, count);
    QueryAnnotationNodeVector ret = (cPtr == IntPtr.Zero) ? null : new QueryAnnotationNodeVector(cPtr, true);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Insert(int index, QueryAnnotationNode x) {
    indri_csharpPINVOKE.QueryAnnotationNodeVector_Insert(swigCPtr, index, QueryAnnotationNode.getCPtr(x));
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void InsertRange(int index, QueryAnnotationNodeVector values) {
    indri_csharpPINVOKE.QueryAnnotationNodeVector_InsertRange(swigCPtr, index, QueryAnnotationNodeVector.getCPtr(values));
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveAt(int index) {
    indri_csharpPINVOKE.QueryAnnotationNodeVector_RemoveAt(swigCPtr, index);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveRange(int index, int count) {
    indri_csharpPINVOKE.QueryAnnotationNodeVector_RemoveRange(swigCPtr, index, count);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public static QueryAnnotationNodeVector Repeat(QueryAnnotationNode value, int count) {
    IntPtr cPtr = indri_csharpPINVOKE.QueryAnnotationNodeVector_Repeat(QueryAnnotationNode.getCPtr(value), count);
    QueryAnnotationNodeVector ret = (cPtr == IntPtr.Zero) ? null : new QueryAnnotationNodeVector(cPtr, true);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Reverse() {
    indri_csharpPINVOKE.QueryAnnotationNodeVector_Reverse__SWIG_0(swigCPtr);
  }

  public void Reverse(int index, int count) {
    indri_csharpPINVOKE.QueryAnnotationNodeVector_Reverse__SWIG_1(swigCPtr, index, count);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetRange(int index, QueryAnnotationNodeVector values) {
    indri_csharpPINVOKE.QueryAnnotationNodeVector_SetRange(swigCPtr, index, QueryAnnotationNodeVector.getCPtr(values));
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public bool Contains(QueryAnnotationNode value) {
    bool ret = indri_csharpPINVOKE.QueryAnnotationNodeVector_Contains(swigCPtr, QueryAnnotationNode.getCPtr(value));
    return ret;
  }

  public int IndexOf(QueryAnnotationNode value) {
    int ret = indri_csharpPINVOKE.QueryAnnotationNodeVector_IndexOf(swigCPtr, QueryAnnotationNode.getCPtr(value));
    return ret;
  }

  public int LastIndexOf(QueryAnnotationNode value) {
    int ret = indri_csharpPINVOKE.QueryAnnotationNodeVector_LastIndexOf(swigCPtr, QueryAnnotationNode.getCPtr(value));
    return ret;
  }

  public bool Remove(QueryAnnotationNode value) {
    bool ret = indri_csharpPINVOKE.QueryAnnotationNodeVector_Remove(swigCPtr, QueryAnnotationNode.getCPtr(value));
    return ret;
  }

}

}
