#pragma once
#include <memory>
#include <string>
#include "assetmanager.h"

template<class T>
class asset_ptr
{
private:
    std::string m_asset_path;
    std::shared_ptr<T> m_shared_asset_ptr;

public:
    asset_ptr()
    : m_asset_path("")
    , m_shared_asset_ptr(nullptr)
    {}

    asset_ptr(const std::string &t_asset_path)
    : m_asset_path("")
    , m_shared_asset_ptr(nullptr)
    {
		m_shared_asset_ptr = asset_manager::get().load_asset<T>(t_asset_path);
    }

    virtual ~asset_ptr()
    {

    }

    T *operator->()
    {
        return *m_shared_asset_ptr;
    }
};