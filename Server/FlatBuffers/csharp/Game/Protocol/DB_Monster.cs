// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace Game.Protocol
{

using global::System;
using global::FlatBuffers;

public struct DB_Monster : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static DB_Monster GetRootAsDB_Monster(ByteBuffer _bb) { return GetRootAsDB_Monster(_bb, new DB_Monster()); }
  public static DB_Monster GetRootAsDB_Monster(ByteBuffer _bb, DB_Monster obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p.bb_pos = _i; __p.bb = _bb; }
  public DB_Monster __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public int Id { get { int o = __p.__offset(4); return o != 0 ? __p.bb.GetInt(o + __p.bb_pos) : (int)0; } }
  public Vec3? Position { get { int o = __p.__offset(6); return o != 0 ? (Vec3?)(new Vec3()).__assign(o + __p.bb_pos, __p.bb) : null; } }
  public int Alive { get { int o = __p.__offset(8); return o != 0 ? __p.bb.GetInt(o + __p.bb_pos) : (int)0; } }
  public Vec3? InitPos { get { int o = __p.__offset(10); return o != 0 ? (Vec3?)(new Vec3()).__assign(o + __p.bb_pos, __p.bb) : null; } }
  public int Hp { get { int o = __p.__offset(12); return o != 0 ? __p.bb.GetInt(o + __p.bb_pos) : (int)0; } }
  public int Maxhp { get { int o = __p.__offset(14); return o != 0 ? __p.bb.GetInt(o + __p.bb_pos) : (int)0; } }

  public static void StartDB_Monster(FlatBufferBuilder builder) { builder.StartObject(6); }
  public static void AddId(FlatBufferBuilder builder, int id) { builder.AddInt(0, id, 0); }
  public static void AddPosition(FlatBufferBuilder builder, Offset<Vec3> positionOffset) { builder.AddStruct(1, positionOffset.Value, 0); }
  public static void AddAlive(FlatBufferBuilder builder, int alive) { builder.AddInt(2, alive, 0); }
  public static void AddInitPos(FlatBufferBuilder builder, Offset<Vec3> initPosOffset) { builder.AddStruct(3, initPosOffset.Value, 0); }
  public static void AddHp(FlatBufferBuilder builder, int hp) { builder.AddInt(4, hp, 0); }
  public static void AddMaxhp(FlatBufferBuilder builder, int maxhp) { builder.AddInt(5, maxhp, 0); }
  public static Offset<DB_Monster> EndDB_Monster(FlatBufferBuilder builder) {
    int o = builder.EndObject();
    return new Offset<DB_Monster>(o);
  }
};


}
