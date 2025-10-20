use luhn_from as lf;

pub trait Luhn {
    fn valid_luhn(&self) -> bool;
}

impl<T> Luhn for T
where
    T: std::fmt::Display,
{
    fn valid_luhn(&self) -> bool {
        lf::Luhn::from(self).is_valid()
    }
}
