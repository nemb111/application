use std::iter::once;

pub fn encrypt(input: &str) -> String {
    if input.is_empty() {
        return "".to_string();
    }
    let mut res_string = input
        .chars()
        .filter(|c| c.is_ascii_alphanumeric())
        .map(|c| c.to_ascii_lowercase())
        .collect::<String>();
    let str_len = res_string.len();
    let square_root = (str_len as f64).sqrt();

    let r = square_root as usize;
    let mut c = square_root as usize;

    if square_root.trunc() < square_root {
        c += 1;
    }

    for _ in str_len..r * c {
        res_string.push(' ');
    }

    debug_assert!(res_string.len() == c * r);
    debug_assert!(c - r <= 1);

    let mut res_string: String = (0..c)
        .flat_map(|n| res_string.chars().skip(n).step_by(c).chain(once(' ')))
        .collect();
    res_string.pop();

    res_string
}
