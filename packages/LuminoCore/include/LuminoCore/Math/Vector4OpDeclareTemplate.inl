
LN_OP_TYPE& operator+=(const LN_OP_TYPE& v) noexcept;
LN_OP_TYPE& operator+=(float v) noexcept;
LN_OP_TYPE& operator-=(const LN_OP_TYPE& v) noexcept;
LN_OP_TYPE& operator-=(float v) noexcept;
LN_OP_TYPE& operator*=(const LN_OP_TYPE& v) noexcept;
LN_OP_TYPE& operator*=(float v) noexcept;
LN_OP_TYPE& operator/=(const LN_OP_TYPE& v) noexcept;
LN_OP_TYPE& operator/=(float v) noexcept;

friend constexpr LN_OP_TYPE operator+(const LN_OP_TYPE& v1, const LN_OP_TYPE& v2) noexcept;
friend constexpr LN_OP_TYPE operator+(const LN_OP_TYPE& v1, float v2) noexcept;
friend constexpr LN_OP_TYPE operator+(float v1, const LN_OP_TYPE& v2) noexcept;
friend constexpr LN_OP_TYPE operator-(const LN_OP_TYPE& v1, const LN_OP_TYPE& v2) noexcept;
friend constexpr LN_OP_TYPE operator-(const LN_OP_TYPE& v1, float v2) noexcept;
friend constexpr LN_OP_TYPE operator-(float v1, const LN_OP_TYPE& v2) noexcept;
friend constexpr LN_OP_TYPE operator*(const LN_OP_TYPE& v1, const LN_OP_TYPE& v2) noexcept;
friend constexpr LN_OP_TYPE operator*(const LN_OP_TYPE& v1, float v2) noexcept;
friend constexpr LN_OP_TYPE operator*(float v1, const LN_OP_TYPE& v2) noexcept;
friend constexpr LN_OP_TYPE operator/(const LN_OP_TYPE& v1, const LN_OP_TYPE& v2) noexcept;
friend constexpr LN_OP_TYPE operator/(const LN_OP_TYPE& v1, float v2) noexcept;
friend constexpr LN_OP_TYPE operator/(float v1, const LN_OP_TYPE& v2) noexcept;

friend constexpr LN_OP_TYPE operator-(const LN_OP_TYPE& v1) noexcept;

constexpr bool operator==(const LN_OP_TYPE& v) const noexcept;
constexpr bool operator!=(const LN_OP_TYPE& v) const noexcept;
