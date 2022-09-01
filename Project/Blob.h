#pragma once
#include <intsafe.h>
#include <string>
#include <vector>

    template<class T>
    using Array = std::vector<T>;
    using Byte = BYTE;
class Blob {
 public:
  using base_type =std::vector<BYTE>;
  using iterator = base_type::iterator;
  using const_iterator = base_type::const_iterator;
  using reverse_iterator = base_type::reverse_iterator;
  using const_reverse_iterator = base_type::const_reverse_iterator;
  
  Blob() = default;
  
  Blob(const Blob&) = default;
  
  Blob(Blob&&) = default;
  explicit Blob(size_t sizeBytes);
  
  
  Blob(const void* src, size_t sizeBytes);
  
  explicit Blob(const Array<Byte>& data);

  explicit Blob(Array<Byte>&& data);

  Blob& operator=(const Array<Byte>& other);

  Blob& operator=(Array<Byte>&& other) noexcept;

  Blob& operator=(const Blob& other);

  Blob& operator=(Blob&& other) noexcept;

  void create(const Blob& other);

  void create(Blob&& other);

  void create(const void* src, size_t sizeBytes);

  void create(const Array<Byte>& data);

  void create(Array<Byte>&& data);
  

  [[nodiscard]] friend bool operator==(const Blob& lhs,
                                       const Blob& rhs) noexcept {
    return (lhs.asArray() == rhs.asArray());
  }

  [[nodiscard]] friend bool operator!=(const Blob& lhs,
                                       const Blob& rhs) noexcept {
    return (lhs.asArray() != rhs.asArray());
  }

  [[nodiscard]] const Byte& operator[](const size_t index) const;

  [[nodiscard]] Byte& operator[](const size_t index);

  [[nodiscard]] const Byte* data() const noexcept;

  [[nodiscard]] Byte* data() noexcept;

  [[nodiscard]] const Array<Byte>& asArray() const noexcept;

  [[nodiscard]] bool empty() const noexcept;

  [[nodiscard]] bool isEmpty() const noexcept;

  [[nodiscard]] explicit operator bool() const noexcept;

  /// @brief バイナリデータのサイズ（バイト）を返します。
  /// @remark `size_bytes()` と同じです。
  /// @return バイナリデータのサイズ（バイト）
  [[nodiscard]] size_t size() const noexcept;

  /// @brief バイナリデータのサイズ（バイト）を返します。
  /// @remark `size` と同じです。
  /// @return バイナリデータのサイズ（バイト）
  [[nodiscard]] size_t size_bytes() const noexcept;

  [[nodiscard]] size_t capacity() const noexcept;

  void reserve(size_t sizeBytes);

  void resize(size_t sizeBytes);

  void shrink_to_fit();

  void clear();

  void release();

  [[nodiscard]] iterator begin() noexcept;

  [[nodiscard]] iterator end() noexcept;

  [[nodiscard]] const_iterator begin() const noexcept;

  [[nodiscard]] const_iterator end() const noexcept;

  [[nodiscard]] const_iterator cbegin() const noexcept;

  [[nodiscard]] const_iterator cend() const noexcept;

  [[nodiscard]] reverse_iterator rbegin() noexcept;

  [[nodiscard]] reverse_iterator rend() noexcept;

  [[nodiscard]] const_reverse_iterator rbegin() const noexcept;

  [[nodiscard]] const_reverse_iterator rend() const noexcept;

  [[nodiscard]] const_reverse_iterator crbegin() const noexcept;

  [[nodiscard]] const_reverse_iterator crend() const noexcept;

  /// @brief 末尾にバイナリデータを追加します。
  /// @param src 追加するデータの先頭ポインタ
  /// @param sizeBytes 追加するデータのサイズ
  void append(const void* src, size_t sizeBytes);
  

  /// @brief バイナリデータを Base64 エンコードします。
  /// @return エンコードされたデータ
  [[nodiscard]] std::string base64() const;
  

  /// @brief バイナリデータを Base64 エンコードし、dst に格納します。
  /// @param dst エンコードされたデータの格納先
  void base64(std::string& dst) const;
  

 private:
  base_type m_data;
};

inline Blob::Blob(const size_t sizeBytes) : m_data(sizeBytes) {}


inline Blob::Blob(const void* src, const size_t sizeBytes)
    : m_data(static_cast<const Byte*>(src),
             (static_cast<const Byte*>(src) + sizeBytes)) {}

inline Blob::Blob(const Array<Byte>& data) : m_data(data) {}

inline Blob::Blob(Array<Byte>&& data) : m_data(std::move(data)) {}

inline Blob& Blob::operator=(const Array<Byte>& other) {
  m_data = other;

  return *this;
}

inline Blob& Blob::operator=(Array<Byte>&& other) noexcept {
  m_data = std::move(other);

  return *this;
}

inline Blob& Blob::operator=(const Blob& other) {
  m_data = other.m_data;

  return *this;
}

inline Blob& Blob::operator=(Blob&& other) noexcept {
  m_data = std::move(other.m_data);

  return *this;
}

inline void Blob::create(const Blob& other) { m_data = other.m_data; }

inline void Blob::create(Blob&& other) { m_data = std::move(other.m_data); }

inline void Blob::create(const void* src, const size_t sizeBytes) {
  m_data.assign(static_cast<const Byte*>(src),
                (static_cast<const Byte*>(src) + sizeBytes));
}

inline void Blob::create(const Array<Byte>& data) { m_data = data; }

inline void Blob::create(Array<Byte>&& data) { m_data = std::move(data); }

inline const Byte& Blob::operator[](const size_t index) const {
  return m_data[index];
}

inline Byte& Blob::operator[](const size_t index) { return m_data[index]; }

inline const Byte* Blob::data() const noexcept { return m_data.data(); }

inline Byte* Blob::data() noexcept { return m_data.data(); }

inline const Array<Byte>& Blob::asArray() const noexcept { return m_data; }

inline bool Blob::empty() const noexcept { return m_data.empty(); }

inline bool Blob::isEmpty() const noexcept { return m_data.empty(); }

inline Blob::operator bool() const noexcept { return (not m_data.empty()); }

inline size_t Blob::size() const noexcept { return m_data.size(); }

inline size_t Blob::size_bytes() const noexcept { return
    (m_data.size() * sizeof(BYTE));
}

inline size_t Blob::capacity() const noexcept { return m_data.capacity(); }

inline void Blob::reserve(const size_t sizeBytes) { m_data.reserve(sizeBytes); }

inline void Blob::resize(const size_t sizeBytes) {
  m_data.resize(sizeBytes, Byte{0});
}

inline void Blob::shrink_to_fit() { m_data.shrink_to_fit(); }

inline void Blob::clear() { m_data.clear(); }

inline Blob::iterator Blob::begin() noexcept { return m_data.begin(); }

inline Blob::iterator Blob::end() noexcept { return m_data.end(); }

inline Blob::const_iterator Blob::begin() const noexcept {
  return m_data.begin();
}

inline Blob::const_iterator Blob::end() const noexcept { return m_data.end(); }

inline Blob::const_iterator Blob::cbegin() const noexcept {
  return m_data.cbegin();
}

inline Blob::const_iterator Blob::cend() const noexcept {
  return m_data.cend();
}

inline Blob::reverse_iterator Blob::rbegin() noexcept {
  return m_data.rbegin();
}

inline Blob::reverse_iterator Blob::rend() noexcept { return m_data.rend(); }

inline Blob::const_reverse_iterator Blob::rbegin() const noexcept {
  return m_data.rbegin();
}

inline Blob::const_reverse_iterator Blob::rend() const noexcept {
  return m_data.rend();
}

inline Blob::const_reverse_iterator Blob::crbegin() const noexcept {
  return m_data.crbegin();
}

inline Blob::const_reverse_iterator Blob::crend() const noexcept {
  return m_data.crend();
}

inline void Blob::append(const void* src, const size_t sizeBytes) {
  m_data.insert(m_data.end(), static_cast<const Byte*>(src),
                (static_cast<const Byte*>(src) + sizeBytes));
}