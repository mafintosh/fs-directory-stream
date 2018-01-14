#include <nan.h>
#include <uv.h>

#define EXPORT_NUMBER(name, num) \
  Nan::Set(target, Nan::New<String>(name).ToLocalChecked(), Nan::New<Number>(num));

#define EXPORT_FUNCTION(name) \
  Nan::Set(target, Nan::New<String>(#name).ToLocalChecked(), Nan::GetFunction(Nan::New<FunctionTemplate>(name)).ToLocalChecked());

#define BUFFER_CAST(type, name, info) \
  type *name = (type *) node::Buffer::Data(info->ToObject());

using namespace v8;

struct fs_directory_stream_t {
  uv_fs_t req;
  Nan::Callback callback;
  uv_dirent_t dirent;

  ~fs_directory_stream_t() {
    uv_fs_req_cleanup(&req);
  }
};

void fs_callback (uv_fs_s* req) {
  Nan::HandleScope scope;

  fs_directory_stream_t *self = (fs_directory_stream_t *) req;

  if (req->result < 0) {
    Local<Value> args[] = {
      Nan::ErrnoException(-req->result, "uv_fs_scandir", NULL, req->path)
    };
    self->callback.Call(1, args);
  } else {
    self->callback.Call(0, NULL);
  }
  self->callback.Reset();
}

// int uv_fs_scandir(uv_loop_t* loop, uv_fs_t* req, const char* path, int flags, uv_fs_cb cb)
NAN_METHOD(uv_fs_scandir) {
  BUFFER_CAST(fs_directory_stream_t, self, info[0])
  BUFFER_CAST(const char, path, info[1])

  Local<Function> cb = info[2].As<Function>();
  self->callback.Reset(cb);

  int err = uv_fs_scandir(uv_default_loop(), &self->req, path, 0, &fs_callback);
  if (err < 0) {
    Nan::ThrowError("uv_fs_scandir failed");
    return;
  }
}

// int uv_fs_scandir_next(uv_fs_t* req, uv_dirent_t* ent)
NAN_METHOD(uv_fs_scandir_next) {
  BUFFER_CAST(fs_directory_stream_t, self, info[0])

  uv_dirent_t dirent;
  Local<Array> names = Nan::New<Array>();

  for (int i = 0; i < 16; i++) {
    int err = uv_fs_scandir_next(&self->req, &dirent);
    if (err == UV_EOF) break;
    if (err < 0) {
      Nan::ThrowError("uv_fs_scandir_next failed");
      return;
    }
    Nan::Set(names, i, Nan::New<String>(dirent.name).ToLocalChecked());
  }

  info.GetReturnValue().Set(names);
}

void free_fs_directory_stream_t(char *data, void *hint) {
  delete (fs_directory_stream_t *) data;
}

NAN_METHOD(make_uv_fs_scandir_buffer) {
  Local<Object> buf = Nan::NewBuffer((char *)new fs_directory_stream_t,
                                     sizeof(fs_directory_stream_t),
                                     free_fs_directory_stream_t,
                                     NULL).ToLocalChecked();
  info.GetReturnValue().Set(buf);
}

NAN_MODULE_INIT(InitAll) {
  EXPORT_FUNCTION(uv_fs_scandir)
  EXPORT_FUNCTION(uv_fs_scandir_next)
  EXPORT_FUNCTION(make_uv_fs_scandir_buffer)
}

NODE_MODULE(fs_directory_stream, InitAll)
