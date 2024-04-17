use std::io::prelude::*;
use std::net::TcpStream;

fn main() -> std::io::Result<()> {
    let mut stream = TcpStream::connect("127.0.0.1:8080").expect("Couldn't connect to server");

    let mut buffer: [u8; 255] = [0; 255];
    stream.write(&buffer)?;

    stream.read(&mut buffer)?;
    println!("Received: {:?}", buffer);

    Ok(())
}
