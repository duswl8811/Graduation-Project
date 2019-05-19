// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace Game.Protocol
{

using global::System;
using global::FlatBuffers;

public struct Monster_Collection : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static Monster_Collection GetRootAsMonster_Collection(ByteBuffer _bb) { return GetRootAsMonster_Collection(_bb, new Monster_Collection()); }
  public static Monster_Collection GetRootAsMonster_Collection(ByteBuffer _bb, Monster_Collection obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p.bb_pos = _i; __p.bb = _bb; }
  public Monster_Collection __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public Monster_info? Data(int j) { int o = __p.__offset(4); return o != 0 ? (Monster_info?)(new Monster_info()).__assign(__p.__indirect(__p.__vector(o) + j * 4), __p.bb) : null; }
  public int DataLength { get { int o = __p.__offset(4); return o != 0 ? __p.__vector_len(o) : 0; } }

  public static Offset<Monster_Collection> CreateMonster_Collection(FlatBufferBuilder builder,
      VectorOffset dataOffset = default(VectorOffset)) {
    builder.StartObject(1);
    Monster_Collection.AddData(builder, dataOffset);
    return Monster_Collection.EndMonster_Collection(builder);
  }

  public static void StartMonster_Collection(FlatBufferBuilder builder) { builder.StartObject(1); }
  public static void AddData(FlatBufferBuilder builder, VectorOffset dataOffset) { builder.AddOffset(0, dataOffset.Value, 0); }
  public static VectorOffset CreateDataVector(FlatBufferBuilder builder, Offset<Monster_info>[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddOffset(data[i].Value); return builder.EndVector(); }
  public static VectorOffset CreateDataVectorBlock(FlatBufferBuilder builder, Offset<Monster_info>[] data) { builder.StartVector(4, data.Length, 4); builder.Add(data); return builder.EndVector(); }
  public static void StartDataVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static Offset<Monster_Collection> EndMonster_Collection(FlatBufferBuilder builder) {
    int o = builder.EndObject();
    return new Offset<Monster_Collection>(o);
  }
};


}
