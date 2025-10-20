use std::cell::Cell;

#[derive(Debug, PartialEq)]
pub enum Error {
    NotEnoughPinsLeft,
    GameComplete,
}

thread_local! {
    pub static COUNTER: Cell<i16> = Cell::new(-1);
}

trait Frame {
    fn add_throw(&mut self, throw: u16) -> Result<(), Error>;
    fn get_frame_count(&self) -> u16;
    fn get_first_throw(&self) -> u16;
    fn get_second_throw(&self) -> u16;
    fn get_third_throw(&self) -> u16 {
        0
    }
    fn get_score(&self) -> u16;

    fn is_finished(&self) -> bool {
        if self.get_first_throw() == 10u16
            || (self.get_first_throw() != u16::max_value()
                && self.get_second_throw() != u16::max_value())
        {
            return true;
        }

        false
    }
    fn is_spare(&self) -> bool {
        Frame::is_finished(self)
            && !Frame::is_strike(self)
            && (self.get_first_throw() + self.get_second_throw()) == 10
    }

    fn is_strike(&self) -> bool {
        self.get_first_throw() == 10
    }
}

struct NormFrame {
    frame_count: u16,
    first_throw: u16,
    second_throw: u16,
}

impl NormFrame {
    fn new() -> Self {
        COUNTER.with(|f| f.set(f.get() + 1));
        Self {
            frame_count: COUNTER.with(|f| f.get() as u16),
            first_throw: u16::max_value(),
            second_throw: u16::max_value(),
        }
    }
}

impl Frame for NormFrame {
    fn add_throw(&mut self, throw: u16) -> Result<(), Error> {
        if throw <= 10u16 {
            if self.first_throw == u16::max_value() {
                self.first_throw = throw;
                if throw == 10 {
                    self.second_throw = 0
                }
                return Ok(());
            } else if self.second_throw == u16::max_value() && throw + self.first_throw <= 10 {
                self.second_throw = throw;
                return Ok(());
            }
        }
        Err(Error::NotEnoughPinsLeft)
    }

    fn get_frame_count(&self) -> u16 {
        self.frame_count
    }

    fn get_first_throw(&self) -> u16 {
        self.first_throw
    }
    fn get_second_throw(&self) -> u16 {
        self.second_throw
    }

    fn get_score(&self) -> u16 {
        self.get_first_throw() + self.get_second_throw()
    }
}

struct TenthFrame {
    norm_frame: NormFrame,
    third_throw: u16,
    forth_throw: u16,
}

impl TenthFrame {
    fn new() -> Self {
        Self {
            norm_frame: NormFrame::new(),
            third_throw: u16::max_value(),
            forth_throw: u16::max_value(),
        }
    }

    fn add_throw_after_norm_frame_is_strike(&mut self, throw: u16) -> Result<(), Error> {
        if self.third_throw == u16::max_value() {
            self.third_throw = throw;
            return Ok(());
        } else if (self.third_throw + throw) <= 10 || self.third_throw == 10 {
            self.forth_throw = throw;
            return Ok(());
        }

        Err(Error::NotEnoughPinsLeft)
    }
}

impl Frame for TenthFrame {
    fn is_finished(&self) -> bool {
        if (!self.norm_frame.is_spare()
            && !self.norm_frame.is_strike()
            && self.norm_frame.is_finished())
            || (self.norm_frame.is_spare() && self.third_throw != u16::max_value())
            || (self.norm_frame.is_strike()
                && self.third_throw != u16::max_value()
                && self.forth_throw != u16::max_value())
        {
            return true;
        }

        false
    }

    fn add_throw(&mut self, throw: u16) -> Result<(), Error> {
        if throw > 10 {
            return Err(Error::NotEnoughPinsLeft);
        }

        if !self.norm_frame.is_finished() {
            let res = self.norm_frame.add_throw(throw);
            if self.norm_frame.is_finished()
                && !self.norm_frame.is_spare()
                && !self.norm_frame.is_strike()
            {
                self.third_throw = 0;
                self.forth_throw = 0;
            }

            return res;
        } else if self.norm_frame.is_spare() {
            if self.third_throw == u16::max_value() {
                self.third_throw = throw;
                self.forth_throw = 0;
                return Ok(());
            }
            return Err(Error::NotEnoughPinsLeft);
        } else if self.norm_frame.is_strike() {
            return self.add_throw_after_norm_frame_is_strike(throw);
        }

        Err(Error::GameComplete)
    }

    fn get_frame_count(&self) -> u16 {
        self.norm_frame.frame_count
    }

    fn get_first_throw(&self) -> u16 {
        self.norm_frame.first_throw
    }
    fn get_second_throw(&self) -> u16 {
        self.norm_frame.second_throw
    }

    fn get_third_throw(&self) -> u16 {
        self.third_throw
    }

    fn get_score(&self) -> u16 {
        self.norm_frame.get_score() + self.third_throw + self.forth_throw
    }
}

pub struct BowlingGame {
    score: Vec<Box<dyn Frame>>,
}

impl BowlingGame {
    pub fn new() -> Self {
        Self {
            score: vec![Box::new(NormFrame::new())],
        }
    }

    pub fn roll(&mut self, pins: u16) -> Result<(), Error> {
        let last = self.score.last_mut();

        if let Some(x) = last {
            if x.get_frame_count() == 9 && x.is_finished() {
                return Err(Error::GameComplete);
            } else if x.is_finished() {
                let mut frame: Box<dyn Frame> = if x.get_frame_count() < 8 {
                    Box::new(NormFrame::new())
                } else {
                    Box::new(TenthFrame::new())
                };
                frame.add_throw(pins)?;
                self.score.push(frame);
            } else {
                x.add_throw(pins)?;
            }
        }

        Ok(())
    }

    pub fn score(&self) -> Option<u16> {
        let last = self.score.last();

        if let Some(x) = last {
            if !(x.get_frame_count() == 9 && x.is_finished()) {
                return None;
            }
        }

        let mut res: u16 = 0u16;

        for i in 0..=7 {
            let e = &self.score[i];
            res += e.get_score();

            if e.is_spare() {
                res += self.score[i + 1].get_first_throw();
            } else if e.is_strike() {
                if self.score[i + 1].is_strike() {
                    res += self.score[i + 1].get_first_throw() + self.score[i + 2].get_first_throw()
                } else {
                    res +=
                        self.score[i + 1].get_first_throw() + self.score[i + 1].get_second_throw();
                }
            }
        }

        res += self.score[8].get_score();
        if self.score[8].is_spare() {
            res += self.score[9].get_first_throw();
        } else if self.score[8].is_strike() {
            if self.score[9].is_strike() {
                res += self.score[9].get_first_throw() + self.score[9].get_third_throw();
            } else {
                res += self.score[9].get_first_throw() + self.score[9].get_second_throw();
            }
        }

        res += self.score[9].get_score();

        Some(res)
    }
}
