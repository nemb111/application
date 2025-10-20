#[derive(Debug, PartialEq)]
pub enum Error {
    SpanTooLong,
    InvalidDigit(char),
}

pub fn lsp(string_digits: &str, span: usize) -> Result<u64, Error> {
    if string_digits.len() < span {
        return Err(Error::SpanTooLong);
    } else if span == 0 {
        return Ok(1);
    }

    let numbers = string_digits
        .chars()
        .map(|c| (c, c.to_digit(10)))
        .map(|(c, n)| match n {
            None => Err(Error::InvalidDigit(c)),
            _ => Ok(n.unwrap() as u8),
        })
        .collect::<Result<Vec<u8>, Error>>()?;

    Ok(numbers
        .windows(span)
        .map(|w| w.iter().map(|n| u64::from(*n)).product())
        .max()
        .unwrap())
}
