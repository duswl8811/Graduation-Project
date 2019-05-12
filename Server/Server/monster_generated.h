// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_MONSTER_MYGAME_MONSTER_H_
#define FLATBUFFERS_GENERATED_MONSTER_MYGAME_MONSTER_H_

#include "flatbuffers/flatbuffers.h"

namespace MyGame {
namespace Monster {

struct Vec3;

struct Monster;

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) Vec3 FLATBUFFERS_FINAL_CLASS {
 private:
  float x_;
  float y_;
  float z_;

 public:
  Vec3() {
    memset(this, 0, sizeof(Vec3));
  }
  Vec3(float _x, float _y, float _z)
      : x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)),
        z_(flatbuffers::EndianScalar(_z)) {
  }
  float x() const {
    return flatbuffers::EndianScalar(x_);
  }
  float y() const {
    return flatbuffers::EndianScalar(y_);
  }
  float z() const {
    return flatbuffers::EndianScalar(z_);
  }
};
FLATBUFFERS_STRUCT_END(Vec3, 12);

struct Monster FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_POS = 4,
    VT_HP = 6
  };
  const Vec3 *pos() const {
    return GetStruct<const Vec3 *>(VT_POS);
  }
  int16_t hp() const {
    return GetField<int16_t>(VT_HP, 100);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<Vec3>(verifier, VT_POS) &&
           VerifyField<int16_t>(verifier, VT_HP) &&
           verifier.EndTable();
  }
};

struct MonsterBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_pos(const Vec3 *pos) {
    fbb_.AddStruct(Monster::VT_POS, pos);
  }
  void add_hp(int16_t hp) {
    fbb_.AddElement<int16_t>(Monster::VT_HP, hp, 100);
  }
  explicit MonsterBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  MonsterBuilder &operator=(const MonsterBuilder &);
  flatbuffers::Offset<Monster> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Monster>(end);
    return o;
  }
};

inline flatbuffers::Offset<Monster> CreateMonster(
    flatbuffers::FlatBufferBuilder &_fbb,
    const Vec3 *pos = 0,
    int16_t hp = 100) {
  MonsterBuilder builder_(_fbb);
  builder_.add_pos(pos);
  builder_.add_hp(hp);
  return builder_.Finish();
}

inline const MyGame::Monster::Monster *GetMonster(const void *buf) {
  return flatbuffers::GetRoot<MyGame::Monster::Monster>(buf);
}

inline const MyGame::Monster::Monster *GetSizePrefixedMonster(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<MyGame::Monster::Monster>(buf);
}

inline bool VerifyMonsterBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<MyGame::Monster::Monster>(nullptr);
}

inline bool VerifySizePrefixedMonsterBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<MyGame::Monster::Monster>(nullptr);
}

inline void FinishMonsterBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<MyGame::Monster::Monster> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedMonsterBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<MyGame::Monster::Monster> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace Monster
}  // namespace MyGame

#endif  // FLATBUFFERS_GENERATED_MONSTER_MYGAME_MONSTER_H_
