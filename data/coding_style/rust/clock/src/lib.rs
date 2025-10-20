use std::fmt;

#[derive(Debug, std::cmp::PartialEq)]
pub struct Clock {
    hours: i32,
    minutes: i32,
}

impl Clock {
    pub fn new(hours: i32, minutes: i32) -> Self {
        let additional_hours = (minutes as f32 / 60f32).floor() as i32;

        Clock {
            hours: (24 + (hours + additional_hours) % 24) % 24,
            minutes: (60 + (minutes % 60)) % 60,
        }
    }

    pub fn add_minutes(&self, minutes: i32) -> Self {
        Clock::new(self.hours, self.minutes + minutes)
    }
}

impl fmt::Display for Clock {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{:02}:{:02}", self.hours, self.minutes)
    }
}
