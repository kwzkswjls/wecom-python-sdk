#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#pragma clang diagnostic ignored "-Wc99-designator"
#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <structmember.h>
#include <string>
#include "WeWorkFinanceSdk_C.h"


auto METH_FLAG = METH_VARARGS | METH_KEYWORDS;

typedef struct {
    PyObject_HEAD
    WeWorkFinanceSdk_t *sdkInstance;
    const char *proxy, *proxyAuth;
} SDKObject;


static void sdk_dealloc(SDKObject *self) {
    DestroySdk(self->sdkInstance);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static int sdk_init(SDKObject *self, PyObject *args, PyObject *kwargs) {

    const char *corpId, *secret;
    const char *kwlist[] = {"corp_id", "secret", "proxy", "proxy_auth", nullptr};
    if (!PyArg_ParseTupleAndKeywords(
            args, kwargs, "ss|ss", const_cast<char **>(kwlist),
            &corpId, &secret,
            &self->proxy, &self->proxyAuth)) {
        return -1;
    }
    self->sdkInstance = NewSdk();
    int ret = Init(self->sdkInstance, corpId, secret);
    if (ret != 0) {
        DestroySdk(self->sdkInstance);
        PyErr_Format(PyExc_RuntimeError, "init sdk error: %d", ret);
        return -1;
    }

    return 0;
}

static PyObject *sdk_getChatData(SDKObject *self, PyObject *args, PyObject *kwargs) {
    uint64_t seq, limit;
    int timeout = 5;
    const char *kwlist[] = {"seq", "limit", "timeout", nullptr};

    if (!PyArg_ParseTupleAndKeywords(
            args, kwargs, "ii|i", const_cast<char **>(kwlist),
            &seq, &limit, self->proxy, self->proxyAuth, &timeout)) {
        return nullptr;
    }

    auto *chatData = NewSlice();
    int ret = GetChatData(
            self->sdkInstance,
            seq, limit,
            self->proxy, self->proxyAuth,
            timeout,
            chatData
    );
    if (ret != 0) {
        PyErr_Format(PyExc_RuntimeError, "get chat data error: %d", ret);
        return nullptr;
    }

    auto res = Py_BuildValue("s", chatData->buf);
    FreeSlice(chatData);

    return res;
}


static PyObject *sdk_getMediaFile(SDKObject *self, PyObject *args, PyObject *kwargs) {

    char *fileId, *filename;
    int timeout = 10;
    const char *kwlist[] = {"file_id", "save_to", "timeout", nullptr};

    if (!PyArg_ParseTupleAndKeywords(
            args, kwargs, "ss|i", const_cast<char **>(kwlist),
            &fileId, &filename, &timeout)) {
        return nullptr;
    }

    std::string index;
    bool isFinish = false;

    while (!isFinish) {
        auto mediaData = NewMediaData();
        int ret = GetMediaData(
                self->sdkInstance,
                index.c_str(),
                fileId,
                self->proxy, self->proxyAuth,
                timeout,
                mediaData
        );

        if (ret != 0) {
            //单个分片拉取失败建议重试拉取该分片，避免从头开始拉取。
            FreeMediaData(mediaData);
            PyErr_Format(PyExc_RuntimeError, "get media data error: %d", ret);
            return nullptr;
        }

        FILE *fp = fopen(filename, "ab+");
        if (fp == nullptr) {
            FreeMediaData(mediaData);
            PyErr_Format(PyExc_RuntimeError, "cannot open file %s", filename);
            return nullptr;
        }

        fwrite(mediaData->data, mediaData->data_len, 1, fp);
        fclose(fp);

        index.assign(std::string(mediaData->outindexbuf));
        isFinish = mediaData->is_finish;
        FreeMediaData(mediaData);
    }

    return Py_None;
}

static PyObject *sdk_decryptData(SDKObject *, PyObject *args, PyObject *kwargs) {
    char *encryptRandomKey, *encryptMsg;
    const char *kwlist[] = {"encrypt_random_key", "encrypt_msg", nullptr};

    if (!PyArg_ParseTupleAndKeywords(
            args, kwargs, "ss", const_cast<char **>(kwlist),
            &encryptRandomKey, &encryptMsg)) {
        return nullptr;
    }

    auto data = NewSlice();
    int ret = DecryptData(encryptRandomKey, encryptMsg, data);
    if (ret != 0) {
        PyErr_Format(PyExc_RuntimeError, "decrypt data error: %d", ret);
        return nullptr;
    }

    auto res = Py_BuildValue("s", data->buf);
    FreeSlice(data);

    return res;
}


static PyMethodDef sdk_methods[] = {
        {"get_chat_data", (PyCFunction) sdk_getChatData, METH_FLAG, "Get chat data"},
        {"get_media_file", (PyCFunction) sdk_getMediaFile, METH_FLAG, "Download media file"},
        {"decrypt_data", (PyCFunction) sdk_decryptData, METH_FLAG, "Decrypt data"},
        {nullptr, nullptr, 0, nullptr}
};

static PyTypeObject SDKType = {
        PyVarObject_HEAD_INIT(nullptr, 0)
        .tp_name="wecom.SDK",
        .tp_basicsize=sizeof(SDKObject),
        .tp_itemsize=0,
        .tp_dealloc=(destructor) sdk_dealloc,
        .tp_methods=sdk_methods,
        .tp_init=(initproc) sdk_init,
        .tp_new=PyType_GenericNew,
};


PyMethodDef method_table[] = {
        {nullptr, nullptr, 0, nullptr}
};

PyModuleDef wecom_module = {
        PyModuleDef_HEAD_INIT,
        "wecom",
        "Python wrap for WeCom C-SDK",
        -1,
        method_table,
};

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
PyMODINIT_FUNC PyInit_wecom(void) {
    PyObject *m;
    if (PyType_Ready(&SDKType) < 0) {
        return nullptr;
    }

    m = PyModule_Create(&wecom_module);
    if (m == nullptr) {
        return nullptr;
    }

    Py_INCREF(&SDKType);
    if (PyModule_AddObject(m, "SDK", (PyObject *) &SDKType) < 0) {
        Py_DECREF(&SDKType);
        Py_DECREF(m);
        return nullptr;
    }

    Py_Initialize();

    return m;
}
#pragma clang diagnostic pop

#pragma clang diagnostic pop