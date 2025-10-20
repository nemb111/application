#[derive(Debug, PartialEq)]
pub enum Error {
    IncompleteNumber,
    Overflow,
}

const MASK: u8 = 0b0111_1111_u8;
const EIGHTH_BIT: u8 = 0b1000_0000_u8;

/// Convert a list of numbers to a stream of bytes encoded with variable length encoding.
pub fn to_bytes(values: &[u32]) -> Vec<u8> {
    let mut res = Vec::with_capacity(values.len());

    for val in values {
        let mut tmp: Vec<u8> = vec![];
        let mut val = *val;
        loop {
            let vlq = (val as u8 & MASK) | EIGHTH_BIT;
            tmp.push(vlq);
            val >>= 7;

            if 0 >= val {
                break;
            }
        }

        tmp[0] -= EIGHTH_BIT;
        res.extend(tmp.iter().rev());
    }

    res
}

/// Given a stream of bytes, extract all numbers which are encoded in there.
pub fn from_bytes(bytes: &[u8]) -> Result<Vec<u32>, Error> {
    let mut bytes_split = Vec::new();

    let mut start = 0_usize;
    for (i, v) in bytes.iter().enumerate() {
        if 0 == (v & EIGHTH_BIT) {
            bytes_split.push(&bytes[start..=i]);
            start = i + 1;
        }
    }
    if bytes_split.is_empty() {
        return Err(Error::IncompleteNumber);
    }

    bytes_split
        .iter()
        .map(|by| {
            by.iter()
                .try_fold(0u32, |acc, x| match acc.checked_mul(128) {
                    None => Err(Error::Overflow),

                    Some(shifted) => match shifted.checked_add((*x & MASK) as u32) {
                        Some(res) => Ok(res),
                        None => Err(Error::Overflow),
                    },
                })
        })
        .collect()
}
