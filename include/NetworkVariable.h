#pragma once

#include <Arduino.h>

#include "FWebServer.h"

class NetworkVariable;

extern NetworkVariable g_varXAxis;
extern NetworkVariable g_varYAxis;


class NetworkVariable
{
public:
    NetworkVariable(const char *name) : _name(name) {}
    NetworkVariable(const char *name, std::function<void(String const&)> callback) : _name(name), _callback(callback) {}

    void init();

    String const& getName() { return _name; }
    String const& getValue() { return _value; }
    int getValueInt() { return _value.toInt(); }

private:
    String _name;
    String _value;
    std::function<void(String const&)> _callback;
};