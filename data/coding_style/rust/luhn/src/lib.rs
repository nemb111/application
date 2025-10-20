/// Check a Luhn checksum.
pub fn is_valid(code: &str) -> bool {
    code.chars()
        .rev()
        .filter(|c| !c.is_ascii_whitespace())
        .try_fold((0, 0), |(i, acc), dig| {
            dig.to_digit(10).map(|mut d| {
                if i % 2 != 0 {
                    d *= 2;
                    if d > 9 {
                        d -= 9;
                    }
                }
                (i + 1, d + acc)
            })
        })
        .map_or(false, |(i, d)| 1 < i && (d % 10) == 0)
}
