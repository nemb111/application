use std::fmt::Display;

pub struct Luhn {
    input: String,
}

impl Luhn {
    pub fn is_valid(&self) -> bool {
        self.input
            .chars()
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
}

impl<T> From<T> for Luhn
where
    T: Display,
{
    fn from(input: T) -> Self {
        Luhn {
            input: input.to_string(),
        }
    }
}
