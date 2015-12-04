// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: samplemessage.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "samplemessage.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace SAMPLE {

namespace {

const ::google::protobuf::Descriptor* Hello_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Hello_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* ID_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_samplemessage_2eproto() {
  protobuf_AddDesc_samplemessage_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "samplemessage.proto");
  GOOGLE_CHECK(file != NULL);
  Hello_descriptor_ = file->message_type(0);
  static const int Hello_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Hello, message_),
  };
  Hello_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      Hello_descriptor_,
      Hello::default_instance_,
      Hello_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Hello, _has_bits_[0]),
      -1,
      -1,
      sizeof(Hello),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Hello, _internal_metadata_),
      -1);
  ID_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_samplemessage_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      Hello_descriptor_, &Hello::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_samplemessage_2eproto() {
  delete Hello::default_instance_;
  delete Hello_reflection_;
}

void protobuf_AddDesc_samplemessage_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\023samplemessage.proto\022\006SAMPLE\"\030\n\005Hello\022\017"
    "\n\007message\030\001 \002(\t*\034\n\002ID\022\013\n\007UNKNOWN\020\000\022\t\n\005HE"
    "LLO\020\001", 85);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "samplemessage.proto", &protobuf_RegisterTypes);
  Hello::default_instance_ = new Hello();
  Hello::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_samplemessage_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_samplemessage_2eproto {
  StaticDescriptorInitializer_samplemessage_2eproto() {
    protobuf_AddDesc_samplemessage_2eproto();
  }
} static_descriptor_initializer_samplemessage_2eproto_;
const ::google::protobuf::EnumDescriptor* ID_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ID_descriptor_;
}
bool ID_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
      return true;
    default:
      return false;
  }
}


namespace {

static void MergeFromFail(int line) GOOGLE_ATTRIBUTE_COLD;
static void MergeFromFail(int line) {
  GOOGLE_CHECK(false) << __FILE__ << ":" << line;
}

}  // namespace


// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int Hello::kMessageFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Hello::Hello()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:SAMPLE.Hello)
}

void Hello::InitAsDefaultInstance() {
}

Hello::Hello(const Hello& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:SAMPLE.Hello)
}

void Hello::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  message_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Hello::~Hello() {
  // @@protoc_insertion_point(destructor:SAMPLE.Hello)
  SharedDtor();
}

void Hello::SharedDtor() {
  message_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (this != default_instance_) {
  }
}

void Hello::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Hello::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Hello_descriptor_;
}

const Hello& Hello::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_samplemessage_2eproto();
  return *default_instance_;
}

Hello* Hello::default_instance_ = NULL;

Hello* Hello::New(::google::protobuf::Arena* arena) const {
  Hello* n = new Hello;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void Hello::Clear() {
  if (has_message()) {
    message_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  if (_internal_metadata_.have_unknown_fields()) {
    mutable_unknown_fields()->Clear();
  }
}

bool Hello::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:SAMPLE.Hello)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string message = 1;
      case 1: {
        if (tag == 10) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_message()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->message().data(), this->message().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "SAMPLE.Hello.message");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:SAMPLE.Hello)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:SAMPLE.Hello)
  return false;
#undef DO_
}

void Hello::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:SAMPLE.Hello)
  // required string message = 1;
  if (has_message()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->message().data(), this->message().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "SAMPLE.Hello.message");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->message(), output);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:SAMPLE.Hello)
}

::google::protobuf::uint8* Hello::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:SAMPLE.Hello)
  // required string message = 1;
  if (has_message()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->message().data(), this->message().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "SAMPLE.Hello.message");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->message(), target);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:SAMPLE.Hello)
  return target;
}

int Hello::ByteSize() const {
  int total_size = 0;

  // required string message = 1;
  if (has_message()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->message());
  }
  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Hello::MergeFrom(const ::google::protobuf::Message& from) {
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  const Hello* source = 
      ::google::protobuf::internal::DynamicCastToGenerated<const Hello>(
          &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Hello::MergeFrom(const Hello& from) {
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_message()) {
      set_has_message();
      message_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.message_);
    }
  }
  if (from._internal_metadata_.have_unknown_fields()) {
    mutable_unknown_fields()->MergeFrom(from.unknown_fields());
  }
}

void Hello::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Hello::CopyFrom(const Hello& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Hello::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void Hello::Swap(Hello* other) {
  if (other == this) return;
  InternalSwap(other);
}
void Hello::InternalSwap(Hello* other) {
  message_.Swap(&other->message_);
  std::swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata Hello::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Hello_descriptor_;
  metadata.reflection = Hello_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// Hello

// required string message = 1;
bool Hello::has_message() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
void Hello::set_has_message() {
  _has_bits_[0] |= 0x00000001u;
}
void Hello::clear_has_message() {
  _has_bits_[0] &= ~0x00000001u;
}
void Hello::clear_message() {
  message_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_message();
}
 const ::std::string& Hello::message() const {
  // @@protoc_insertion_point(field_get:SAMPLE.Hello.message)
  return message_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 void Hello::set_message(const ::std::string& value) {
  set_has_message();
  message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:SAMPLE.Hello.message)
}
 void Hello::set_message(const char* value) {
  set_has_message();
  message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:SAMPLE.Hello.message)
}
 void Hello::set_message(const char* value, size_t size) {
  set_has_message();
  message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:SAMPLE.Hello.message)
}
 ::std::string* Hello::mutable_message() {
  set_has_message();
  // @@protoc_insertion_point(field_mutable:SAMPLE.Hello.message)
  return message_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 ::std::string* Hello::release_message() {
  clear_has_message();
  return message_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 void Hello::set_allocated_message(::std::string* message) {
  if (message != NULL) {
    set_has_message();
  } else {
    clear_has_message();
  }
  message_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), message);
  // @@protoc_insertion_point(field_set_allocated:SAMPLE.Hello.message)
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace SAMPLE

// @@protoc_insertion_point(global_scope)
