use std::collections::HashMap;

pub type Value = i32;
pub type ForthResult = Result<(), Error>;

#[derive(Default)]
pub struct Forth {
    stack: Vec<Value>,
    user_def_words: HashMap<String, Vec<Tokens>>,
}

#[derive(Debug, PartialEq)]
pub enum Error {
    DivisionByZero,
    StackUnderflow,
    UnknownWord,
    InvalidWord,
}

#[derive(PartialEq, Debug, Clone)]
enum Tokens {
    Literal(Value),
    Word(String),
    UserDefWord {
        word_name: String,
        word_body: Vec<Tokens>,
    },
}

impl Forth {
    pub fn new() -> Forth {
        Default::default()
    }

    pub fn stack(&self) -> Vec<Value> {
        self.stack.clone()
    }

    pub fn eval(&mut self, input: &str) -> ForthResult {
        let split_input = input.split_ascii_whitespace();
        let mut owned_split_input = split_input.map(|s| s.to_ascii_lowercase());

        let tokens = Forth::tokenize(&mut owned_split_input)?;

        self.parse(tokens.iter())?;

        Ok(())
    }

    fn parse<'a, I>(&mut self, tokens: I) -> ForthResult
    where
        I: IntoIterator<Item = &'a Tokens>,
    {
        for t in tokens {
            match t {
                Tokens::Literal(_) => self.parse_literal(&t)?,
                Tokens::Word(_) => self.parse_word(&t)?,
                Tokens::UserDefWord {
                    word_name: _,
                    word_body: _,
                } => self.parse_user_def_word(&t)?,
            };
        }

        Ok(())
    }

    fn tokenize<I>(input: &mut I) -> Result<Vec<Tokens>, Error>
    where
        I: Iterator<Item = String>,
    {
        let mut res = Vec::with_capacity(16);

        while let Some(i) = input.next() {
            res.push(match (&i[..], i.parse::<i32>()) {
                (_, Ok(literal)) => Tokens::Literal(literal),
                (":", Err(_)) => Self::create_user_def_word_token(input)?,
                _ => Tokens::Word(i),
            });
        }

        Ok(res)
    }

    fn create_user_def_word_token<I>(lowercase_split_input: &mut I) -> Result<Tokens, Error>
    where
        I: Iterator<Item = String>,
    {
        let word_name = lowercase_split_input.next();

        if word_name.is_none() {
            return Err(Error::InvalidWord);
        }
        let word_name = word_name.unwrap();

        if word_name.parse::<f64>().is_ok() {
            return Err(Error::InvalidWord);
        }

        let mut word_body: Vec<String> = vec![];
        let mut semicolon_terminated = false;

        for s in lowercase_split_input {
            if s == ";" {
                semicolon_terminated = true;
                break;
            }

            word_body.push(s);
        }

        let word_body = Self::tokenize(&mut word_body.into_iter())?;

        if !semicolon_terminated || word_body.is_empty() {
            Err(Error::InvalidWord)
        } else {
            Ok(Tokens::UserDefWord {
                word_name,
                word_body,
            })
        }
    }

    fn parse_literal(&mut self, token: &Tokens) -> ForthResult {
        match token {
            Tokens::Literal(t) => self.stack.push(*t),
            _ => unreachable!(),
        }

        Ok(())
    }

    fn parse_word(&mut self, token: &Tokens) -> ForthResult {
        let word = match token {
            Tokens::Word(t) => t,
            _ => unreachable!(),
        };

        match self.user_def_words.get(word).cloned() {
            None => match word.as_str() {
                "+" => self.exec_add(),
                "-" => self.exec_sub(),
                "*" => self.exec_mul(),
                "/" => self.exec_div(),
                "dup" => self.exec_dup(),
                "drop" => self.exec_drop(),
                "swap" => self.exec_swap(),
                "over" => self.exec_over(),
                _ => Err(Error::UnknownWord),
            },
            Some(tokens) => self.parse(&tokens),
        }
    }

    fn parse_user_def_word(&mut self, token: &Tokens) -> ForthResult {
        let (word_name, word_body) = match token {
            Tokens::UserDefWord {
                word_name: n,
                word_body: b,
            } => (n, b),
            _ => unreachable!(),
        };

        let mut tmp: Vec<_> = vec![];
        for t in word_body {
            if let Tokens::Word(name) = t {
                if let Some(v) = self.user_def_words.get(name) {
                    for t2 in v {
                        tmp.push(t2.clone());
                    }
                } else {
                    tmp.push(t.clone());
                }
            } else {
                tmp.push(t.clone());
            }
        }

        self.user_def_words.insert(word_name.clone(), tmp);

        Ok(())
    }

    fn exec_add(&mut self) -> ForthResult {
        let b = match self.stack.pop() {
            Some(b) => b,
            None => return Err(Error::StackUnderflow),
        };

        let a = match self.stack.pop() {
            Some(a) => a,
            None => return Err(Error::StackUnderflow),
        };

        self.stack.push(a + b);

        Ok(())
    }

    fn exec_sub(&mut self) -> ForthResult {
        let b = match self.stack.pop() {
            Some(b) => b,
            None => return Err(Error::StackUnderflow),
        };

        let a = match self.stack.pop() {
            Some(a) => a,
            None => return Err(Error::StackUnderflow),
        };

        self.stack.push(a - b);

        Ok(())
    }

    fn exec_mul(&mut self) -> ForthResult {
        let b = match self.stack.pop() {
            Some(b) => b,
            None => return Err(Error::StackUnderflow),
        };

        let a = match self.stack.pop() {
            Some(a) => a,
            None => return Err(Error::StackUnderflow),
        };

        self.stack.push(a * b);

        Ok(())
    }

    fn exec_div(&mut self) -> ForthResult {
        let b = match self.stack.pop() {
            Some(b) => b,
            None => return Err(Error::StackUnderflow),
        };

        if b == 0 {
            return Err(Error::DivisionByZero);
        }

        let a = match self.stack.pop() {
            Some(a) => a,
            None => return Err(Error::StackUnderflow),
        };

        self.stack.push(a / b);

        Ok(())
    }

    fn exec_dup(&mut self) -> ForthResult {
        let a = match self.stack.last().cloned() {
            Some(a) => a,
            None => return Err(Error::StackUnderflow),
        };
        self.stack.push(a);

        Ok(())
    }

    fn exec_drop(&mut self) -> ForthResult {
        if self.stack.pop().is_none() {
            return Err(Error::StackUnderflow);
        };

        Ok(())
    }

    fn exec_swap(&mut self) -> ForthResult {
        let b = match self.stack.pop() {
            Some(b) => b,
            None => return Err(Error::StackUnderflow),
        };

        let a = match self.stack.pop() {
            Some(a) => a,
            None => return Err(Error::StackUnderflow),
        };

        self.stack.push(b);
        self.stack.push(a);

        Ok(())
    }

    fn exec_over(&mut self) -> ForthResult {
        if self.stack.len() >= 2 {
            let a = self.stack[self.stack.len() - 2];
            self.stack.push(a);
        } else {
            return Err(Error::StackUnderflow);
        }

        Ok(())
    }
}
