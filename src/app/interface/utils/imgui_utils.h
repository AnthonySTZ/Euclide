#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <string>

inline ImVec2 operator+(const ImVec2 &lvec, const ImVec2 &rvec) {
    return ImVec2(lvec.x + rvec.x, lvec.y + rvec.y);
}

inline ImVec2& operator+=(ImVec2& lvec, const ImVec2& rvec) {
    lvec.x += rvec.x;
    lvec.y += rvec.y;
    return lvec;
}

inline ImVec2 operator-(const ImVec2 &lvec, const ImVec2 &rvec) {
    return ImVec2(lvec.x - rvec.x, lvec.y - rvec.y);
}

inline ImVec2& operator-=(ImVec2& lvec, const ImVec2& rvec) {
    lvec.x -= rvec.x;
    lvec.y -= rvec.y;
    return lvec;
}

inline ImVec2 operator*(const ImVec2 &lvec, const float &rval) {
    return ImVec2(lvec.x * rval, lvec.y * rval);
}

inline ImVec4 operator*(const ImVec4 &lvec, const float &rval) {
    return ImVec4(lvec.x * rval, lvec.y * rval, lvec.z * rval, lvec.w);
}

inline bool operator==(const ImVec2 &lvec, const ImVec2 &rvec) {
    return (lvec.x == rvec.x) && (lvec.y == rvec.y);
}

inline bool operator!=(const ImVec2 &lvec, const ImVec2 &rvec) {
    return !(lvec == rvec);
}

static int StringImGuiCallBack(ImGuiInputTextCallbackData *data) {
    /* Used to resize the string for a imGui input text*/
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
        std::string *str = (std::string *)data->UserData;
        str->resize(data->BufTextLen);
        data->Buf = (char *)str->c_str();
    }
    return 0;
}

static bool ccw(ImVec2 A, ImVec2 B, ImVec2 C) {
	return (C.y - A.y) * (B.x - A.x) > (B.y - A.y) * (C.x - A.x);
}

static bool lineIntersect(ImVec2 A, ImVec2 B, ImVec2 C, ImVec2 D) {
	return ccw(A, C, D) != ccw(B, C, D) && ccw(A, B, C) != ccw(A, B, D);
}
