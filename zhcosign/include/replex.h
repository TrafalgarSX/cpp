#ifndef REPLEX_H
#define REPLEX_H

#if defined(_WIN32)
#include <windows.h>
#include <libloaderapi.h>
#else
#include <dlfcn.h>
#endif


#include <array>
#include <iostream>
#include <string>
#include <unordered_map>

template <typename E, size_t NumSymbols> class ReplexModule {
public:
  static void LoadLib() { GetInstance().Load(); }
  static void ReloadLib() { GetInstance().Reload(); }
  static void UnloadLib() { GetInstance().Unload(); }

  virtual ~ReplexModule() {}

protected:
  static E &GetInstance() {
    static E instance;
    return instance;
  }

  // Should return the path to the library on disk
  const char *GetPath() const {
    return static_cast<const E*>(this)->GetPathImpl();
  }

  // Should return a reference to an array of C-strings of size NumSymbols
  // Used when loading or reloading the library to lookup the address of
  // all exported symbols
  std::array<const char *, NumSymbols> &GetSymbolNames() const {
    return static_cast<const E*>(this)->GetSymbolNamesImpl();
  }

  template <typename Ret, typename... Args>
  Ret Execute(const char *name, Args... args) {
    // Lookup the function address
    auto symbol = m_symbols.find(name);
    if (symbol != m_symbols.end()) {
      // Cast the address to the appropriate function type and call it,
      // forwarding all arguments
      return reinterpret_cast<Ret (*)(Args...)>(symbol->second)(args...);
    }
    throw std::runtime_error(std::string("Function not found: ") + name);
  }

  template <typename T> T *GetVar(const char *name) {
    auto symbol = m_symbols.find(name);
    if (symbol != m_symbols.end()) {
      return static_cast<T *>(symbol->second);
    }
    // We didn't find the variable. Return an empty pointer
    return nullptr;
  }

private:
  void Load() {
#ifdef _WIN32
    m_libHandle = LoadLibrary(GetPath());
#else
    m_libHandle = dlopen(GetPath(), RTLD_NOW);
#endif
    LoadSymbols();
  }

  void Reload() {
#ifdef _WIN32
    FreeLibrary(static_cast<HMODULE>(m_libHandle));
#else
    dlclose(m_libHandle);
#endif
    m_symbols.clear();
    Load();
  }

  void Unload() {
#ifdef _WIN32
    FreeLibrary(static_cast<HMODULE>(m_libHandle));
#else
    dlclose(m_libHandle);
#endif
    m_symbols.clear();
    m_libHandle = nullptr;
  }

  void LoadSymbols() {
    for (const char *symbol : GetSymbolNames()) {
#ifdef _WIN32
      auto *sym = GetProcAddress(static_cast<HMODULE>(m_libHandle), symbol);
#else
      auto *sym = dlsym(m_libHandle, symbol);
#endif
      m_symbols.insert({symbol, reinterpret_cast<void *>(sym)});
    }
  }

  void *m_libHandle = nullptr;
  std::unordered_map<std::string, void *> m_symbols;
};

#endif // REPLEX_H