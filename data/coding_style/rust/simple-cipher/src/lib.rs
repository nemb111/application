pub fn encode(key: &str, s: &str) -> Option<String> {
    let rot = |c: char, base: u8, key: char| -> char {
        (((c as u8 + key as u8 - 2 * base) % 26) + base) as char
    };

    common_encode_decode_logic(key, s, rot)
}

pub fn decode(key: &str, s: &str) -> Option<String> {
    let derot = |c: char, base: u8, key: char| -> char {
        (((c as u8 + 26 - key as u8) % 26) + base) as char
    };

    common_encode_decode_logic(key, s, derot)
}

pub fn encode_random(s: &str) -> (String, String) {
    use rand::distributions::{Distribution, Uniform};

    let mut rng = rand::thread_rng();
    let die = Uniform::from(b'a'..=b'z');

    let key: String = (0..std::cmp::max(100, s.len()))
        .map(|_| die.sample(&mut rng) as u8 as char)
        .collect();

    let s = encode(&key, s).unwrap();

    (key, s)
}

fn common_encode_decode_logic(
    key: &str,
    s: &str,
    rot_or_derot: impl Fn(char, u8, char) -> char,
) -> Option<String> {
    if key.is_empty() {
        return None;
    }

    s.chars()
        .zip(key.chars().cycle())
        .map(|(c, k)| {
            if k.is_ascii_lowercase() {
                return Some((c, k));
            }
            None
        })
        .map(|el| {
            el.map(|(c, k)| {
                if c.is_ascii_lowercase() {
                    return rot_or_derot(c, b'a', k);
                } else if c.is_ascii_uppercase() {
                    return rot_or_derot(c, b'A', k);
                }
                c
            })
        })
        .collect()
}
