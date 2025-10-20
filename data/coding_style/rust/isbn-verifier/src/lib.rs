/// Determines whether the supplied string is a valid ISBN number
pub fn is_valid_isbn(isbn: &str) -> bool {
    let isbn = isbn
        .chars()
        .filter(|&c| c.is_ascii_digit() || c == 'X')
        .enumerate()
        .filter_map(|(i, c)| match c {
            '0'..='9' => c.to_digit(10),
            'X' if i == 9 => Some(10),
            _ => None,
        });

    if isbn.clone().count() != 10 {
        return false;
    }

    isbn.zip((1..=10).rev()).map(|(a, b)| a * b).sum::<u32>() % 11 == 0
}
