// The code below is a stub. Just enough to satisfy the compiler.
// In order to pass the tests you can add-to or change any of this code.

#[derive(Debug)]
pub struct Duration {
    seconds: u64,
}

impl From<u64> for Duration {
    fn from(s: u64) -> Self {
        Self { seconds: s }
    }
}

pub trait Planet {
    const EARTH_YEARS_PER_ORBITAL_PERIOD: f64;

    fn years_during(d: &Duration) -> f64 {
        const SECONDS_PER_ORBITAL_PERIOD: u64 = 31_557_600;

        (d.seconds as f64 / SECONDS_PER_ORBITAL_PERIOD as f64)
            / Self::EARTH_YEARS_PER_ORBITAL_PERIOD
    }
}

macro_rules! impl_planet {
    ($planet_name:ident, $EARTH_YEARS_PER_ORBITAL_PERIOD:expr) => {
        pub struct $planet_name;
        impl Planet for $planet_name {
            const EARTH_YEARS_PER_ORBITAL_PERIOD: f64 = $EARTH_YEARS_PER_ORBITAL_PERIOD;
        }
    };
}

impl_planet!(Mercury, 0.240_846_7_f64);
impl_planet!(Venus, 0.615_197_26_f64);
impl_planet!(Earth, 1_f64);
impl_planet!(Mars, 1.880_815_8_f64);
impl_planet!(Jupiter, 11.862_615_f64);
impl_planet!(Saturn, 29.447_498_f64);
impl_planet!(Uranus, 84.016_846_f64);
impl_planet!(Neptune, 164.791_32_f64);
