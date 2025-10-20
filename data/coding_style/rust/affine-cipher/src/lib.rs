/// While the problem description indicates a return status of 1 should be returned on errors,
/// it is much more common to return a `Result`, so we provide an error type for the result here.
#[derive(Debug, Eq, PartialEq)]
pub enum AffineCipherError {
    NotCoprime(i32),
}

/// Encodes the plaintext using the affine cipher with key (`a`, `b`). Note that, rather than
/// returning a return code, the more common convention in Rust is to return a `Result`.
pub fn encode(plaintext: &str, a: i32, b: i32) -> Result<String, AffineCipherError> {
    let (d, _, _) = ext_euclid(a.into(), 26);
    if d != 1 {
        return Err(AffineCipherError::NotCoprime(a));
    }

    let mut res = plaintext
        .chars()
        .filter(|c| c.is_ascii_alphanumeric())
        .map(|c| c.to_ascii_lowercase())
        .map(|c| {
            if c.is_ascii_alphabetic() {
                return ((a * (c as i32 - b'a' as i32) + b) % 26 + b'a' as i32) as u8 as char;
            }
            c
        })
        .collect::<Vec<char>>()
        .chunks(5)
        .flat_map(|ch| ch.iter().copied().chain(std::iter::once(' ')))
        .collect::<String>();
    res.pop();

    Ok(res)
}

/// Decodes the ciphertext using the affine cipher with key (`a`, `b`). Note that, rather than
/// returning a return code, the more common convention in Rust is to return a `Result`.
pub fn decode(ciphertext: &str, a: i32, b: i32) -> Result<String, AffineCipherError> {
    let (d, s, _) = ext_euclid(a.into(), 26);
    if d != 1 {
        return Err(AffineCipherError::NotCoprime(a));
    }
    let s = (s + 26) % 26;

    Ok(ciphertext
        .chars()
        .filter(|c| c.is_ascii_alphanumeric())
        .map(|c| c.to_ascii_lowercase())
        .map(|c| {
            if c.is_ascii_alphabetic() {
                return ((s as i32 * ((((c as i32 - b'a' as i32) - b) % 26) + 26)) % 26
                    + b'a' as i32) as u8 as char;
            }
            c
        })
        .collect())
}

fn ext_euclid(a: i64, b: i64) -> (i64, i64, i64) {
    if b == 0 {
        return (a, 1, 0);
    }
    let (d, mmi_a, mmi_b) = ext_euclid(b, a % b);
    let (d, mmi_a, mmi_b) = (d, mmi_b, mmi_a - (a / b) * mmi_b);

    (d, mmi_a, mmi_b)
}
