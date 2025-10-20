pub struct CircularBuffer<T> {
    buffer: Box<[Option<T>]>,
    read_index: usize,
    write_index: usize,
}

#[derive(Debug, PartialEq, Eq)]
pub enum Error {
    EmptyBuffer,
    FullBuffer,
}

impl<T: Clone> CircularBuffer<T> {
    pub fn new(capacity: usize) -> Self {
        CircularBuffer {
            buffer: vec![None; capacity].into_boxed_slice(),
            read_index: 0,
            write_index: 0,
        }
    }

    pub fn write(&mut self, element: T) -> Result<(), Error> {
        if self.read_index == self.write_index && self.buffer[self.read_index].is_some() {
            // Buffer is full
            return Err(Error::FullBuffer);
        }

        self.buffer[self.write_index] = Some(element);
        self.write_index = (self.write_index + 1) % self.buffer.len();

        Ok(())
    }

    pub fn read(&mut self) -> Result<T, Error> {
        if let Some(element) = self.buffer[self.read_index].take() {
            // Clear the element
            self.read_index = (self.read_index + 1) % self.buffer.len();

            Ok(element)
        } else {
            Err(Error::EmptyBuffer)
        }
    }

    pub fn clear(&mut self) {
        while {
            self.buffer[self.read_index] = None;

            let res = self.read_index != self.write_index;

            if res {
                self.read_index = (self.read_index + 1) % self.buffer.len();
            }

            res
        } {}
    }

    pub fn overwrite(&mut self, element: T) {
        if self.read_index == self.write_index && self.buffer[self.read_index].is_some() {
            // Buffer is full overwrite the oldest element.
            self.read_index = (self.read_index + 1) % self.buffer.len();
        }

        let _ = self.write(element);
    }
}
