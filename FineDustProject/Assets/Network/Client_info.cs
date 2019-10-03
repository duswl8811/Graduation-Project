// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace Game.Protocol
{

using global::System;
using global::FlatBuffers;

public struct Client_info : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static Client_info GetRootAsClient_info(ByteBuffer _bb) { return GetRootAsClient_info(_bb, new Client_info()); }
  public static Client_info GetRootAsClient_info(ByteBuffer _bb, Client_info obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p.bb_pos = _i; __p.bb = _bb; }
  public Client_info __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public int Id { get { int o = __p.__offset(4); return o != 0 ? __p.bb.GetInt(o + __p.bb_pos) : (int)0; } }
  public int Type { get { int o = __p.__offset(6); return o != 0 ? __p.bb.GetInt(o + __p.bb_pos) : (int)0; } }
  public int Hp { get { int o = __p.__offset(8); return o != 0 ? __p.bb.GetInt(o + __p.bb_pos) : (int)0; } }
  public int Animator { get { int o = __p.__offset(10); return o != 0 ? __p.bb.GetInt(o + __p.bb_pos) : (int)0; } }
  public float DirX { get { int o = __p.__offset(12); return o != 0 ? __p.bb.GetFloat(o + __p.bb_pos) : (float)0.0f; } }
  public float DirZ { get { int o = __p.__offset(14); return o != 0 ? __p.bb.GetFloat(o + __p.bb_pos) : (float)0.0f; } }
  public float Horizontal { get { int o = __p.__offset(16); return o != 0 ? __p.bb.GetFloat(o + __p.bb_pos) : (float)0.0f; } }
  public float Vertical { get { int o = __p.__offset(18); return o != 0 ? __p.bb.GetFloat(o + __p.bb_pos) : (float)0.0f; } }
  public string Name { get { int o = __p.__offset(20); return o != 0 ? __p.__string(o + __p.bb_pos) : null; } }
#if ENABLE_SPAN_T
  public Span<byte> GetNameBytes() { return __p.__vector_as_span(20); }
#else
  public ArraySegment<byte>? GetNameBytes() { return __p.__vector_as_arraysegment(20); }
#endif
  public byte[] GetNameArray() { return __p.__vector_as_array<byte>(20); }
  public Vec3? Position { get { int o = __p.__offset(22); return o != 0 ? (Vec3?)(new Vec3()).__assign(o + __p.bb_pos, __p.bb) : null; } }
  public Vec3? Rotation { get { int o = __p.__offset(24); return o != 0 ? (Vec3?)(new Vec3()).__assign(o + __p.bb_pos, __p.bb) : null; } }

  public static void StartClient_info(FlatBufferBuilder builder) { builder.StartObject(11); }
  public static void AddId(FlatBufferBuilder builder, int id) { builder.AddInt(0, id, 0); }
  public static void AddType(FlatBufferBuilder builder, int type) { builder.AddInt(1, type, 0); }
  public static void AddHp(FlatBufferBuilder builder, int hp) { builder.AddInt(2, hp, 0); }
  public static void AddAnimator(FlatBufferBuilder builder, int animator) { builder.AddInt(3, animator, 0); }
  public static void AddDirX(FlatBufferBuilder builder, float dirX) { builder.AddFloat(4, dirX, 0.0f); }
  public static void AddDirZ(FlatBufferBuilder builder, float dirZ) { builder.AddFloat(5, dirZ, 0.0f); }
  public static void AddHorizontal(FlatBufferBuilder builder, float horizontal) { builder.AddFloat(6, horizontal, 0.0f); }
  public static void AddVertical(FlatBufferBuilder builder, float vertical) { builder.AddFloat(7, vertical, 0.0f); }
  public static void AddName(FlatBufferBuilder builder, StringOffset nameOffset) { builder.AddOffset(8, nameOffset.Value, 0); }
  public static void AddPosition(FlatBufferBuilder builder, Offset<Vec3> positionOffset) { builder.AddStruct(9, positionOffset.Value, 0); }
  public static void AddRotation(FlatBufferBuilder builder, Offset<Vec3> rotationOffset) { builder.AddStruct(10, rotationOffset.Value, 0); }
  public static Offset<Client_info> EndClient_info(FlatBufferBuilder builder) {
    int o = builder.EndObject();
    return new Offset<Client_info>(o);
  }
};


}
