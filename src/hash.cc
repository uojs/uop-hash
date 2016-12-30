#include <nan.h>

v8::Handle<v8::Array> LongToArray(uint32_t first, uint32_t second) {
    Nan::EscapableHandleScope scope;
    v8::Handle<v8::Array> array = Nan::New<v8::Array>();

    array->Set(0, Nan::New(first));
    array->Set(1, Nan::New(second));

    return scope.Escape(array);
}

void Hash(const Nan::FunctionCallbackInfo<v8::Value>& args) {
    if (args.Length() != 1) {
        Nan::ThrowTypeError("requires one argument");
        return;
    }

    if (!args[0]->IsString()) {
        Nan::ThrowTypeError("argument must be of type string");
        return;
    }

    v8::String::Utf8Value str(args[0]->ToString());
    const char* s = *str;
    uint32_t eax, ecx, edx, ebx, esi, edi, length = str.length();

    eax = ecx = edx = ebx = esi = edi = 0;
    ebx = edi = esi = (uint32_t)length + 0xDEADBEEF;

    uint32_t i = 0;
    int32_t j = 0;
    for (i = 0; i + 12 < length; i += 12)
    {
        edi = (uint32_t)((s[i + 7] << 24) | (s[i + 6] << 16) | (s[i + 5] << 8) | s[i + 4]) + edi;
        esi = (uint32_t)((s[i + 11] << 24) | (s[i + 10] << 16) | (s[i + 9] << 8) | s[i + 8]) + esi;
        edx = (uint32_t)((s[i + 3] << 24) | (s[i + 2] << 16) | (s[i + 1] << 8) | s[i]) - esi;

        edx = (edx + ebx) ^ (esi >> 28) ^ (esi << 4);
        esi += edi;
        edi = (edi - edx) ^ (edx >> 26) ^ (edx << 6);
        edx += esi;
        esi = (esi - edi) ^ (edi >> 24) ^ (edi << 8);
        edi += edx;
        ebx = (edx - esi) ^ (esi >> 16) ^ (esi << 16);
        esi += edi;
        edi = (edi - ebx) ^ (ebx >> 13) ^ (ebx << 19);
        ebx += esi;
        esi = (esi - edi) ^ (edi >> 28) ^ (edi << 4);
        edi += ebx;
    }

    j = length - i;
    if (j > 0)
    {
        switch (j)
        {
            case 12:
                esi += (uint32_t)s[i + 11] << 24;
            case 11:
                esi += (uint32_t)s[i + 10] << 16;
            case 10:
                esi += (uint32_t)s[i + 9] << 8;
            case 9:
                esi += s[i + 8];
            case 8:
                edi += (uint32_t)s[i + 7] << 24;
            case 7:
                edi += (uint32_t)s[i + 6] << 16;
            case 6:
                edi += (uint32_t)s[i + 5] << 8;
            case 5:
                edi += s[i + 4];
            case 4:
                ebx += (uint32_t)s[i + 3] << 24;
            case 3:
                ebx += (uint32_t)s[i + 2] << 16;
            case 2:
                ebx += (uint32_t)s[i + 1] << 8;
            case 1:
                ebx += s[i];
                break;
        }

        esi = (esi ^ edi) - ((edi >> 18) ^ (edi << 14));
        ecx = (esi ^ ebx) - ((esi >> 21) ^ (esi << 11));
        edi = (edi ^ ecx) - ((ecx >> 7) ^ (ecx << 25));
        esi = (esi ^ edi) - ((edi >> 16) ^ (edi << 16));
        edx = (esi ^ ecx) - ((esi >> 28) ^ (esi << 4));
        edi = (edi ^ edx) - ((edx >> 18) ^ (edx << 14));
        eax = (esi ^ edi) - ((edi >> 8) ^ (edi << 24));
        args.GetReturnValue().Set(LongToArray(edi, eax));
        return;
    }

    args.GetReturnValue().Set(LongToArray(esi, eax));
}

void Init(v8::Local<v8::Object> exports) {
    exports->Set(Nan::New("hash").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(Hash)->GetFunction());
}

NODE_MODULE(hash, Init)
