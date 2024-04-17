use std::io::prelude::*;
use std::net::{TcpListener, TcpStream};

fn handle_client(mut stream: TcpStream) -> std::io::Result<()> {
    // handle client
    let mut buffer: [u8; 255] = [0; 255];
    while stream.read(&mut buffer)? > 0 {
        println!("Received: {:?}", buffer);
        stream.write(&buffer)?;
        println!("Sent: {:?}", buffer.len());
    }

    Ok(())
}

fn main() -> std::io::Result<()> {
    let listener = TcpListener::bind("127.0.0.1:8080").unwrap();

    match listener.accept() {
        Ok((stream, addr)) => {
            println!("Connection from: {}", addr);
            handle_client(stream)
        }
        Err(e) => {
            println!("Failed to establish a connection: {}", e);
            Err(e)
        }
    }
}
