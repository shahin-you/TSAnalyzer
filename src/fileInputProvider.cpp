#include "fileInputProvider.h"

FileInputProvider::FileInputProvider(const std::string& filename, size_t bufferSizeInPackets) 
: m_filename(filename), 
  m_isOpen(false), 
  m_fileSize(0),
  m_totalPackets(0),
  m_processedPackets(0),
  m_bufferSizeInPackets(bufferSizeInPackets),
  m_packetCallback(nullptr)
{
    // Allocate buffer for reading multiple packets at once
    m_buffer.resize(m_bufferSizeInPackets * TS_PACKET_SIZE);
}

void FileInputProvider::setPacketCallback(TSPacketCallback callback)
{
    m_packetCallback = callback;
}

bool FileInputProvider::open()
{
    // Ensure we have a callback set
    if (!m_packetCallback)
    {
        std::cerr << "Error: No packet callback set" << std::endl;
        return false;
    }
    
    // If already open, close first
    if (m_isOpen)
    {
        close();
    }
    
    // Open file in binary mode
    m_file.open(m_filename, std::ios::binary | std::ios::in);
    if (!m_file.is_open())
    {
        std::cerr << "Error: Could not open file " << m_filename << std::endl;
        return false;
    }
    
    // Get file size
    m_file.seekg(0, std::ios::end);
    m_fileSize = m_file.tellg();
    m_file.seekg(0, std::ios::beg);
    
    // Calculate total number of full packets
    m_totalPackets = m_fileSize / TS_PACKET_SIZE;
    m_processedPackets = 0;
    
    m_isOpen = true;
    
    // Verify that file size is a multiple of TS packet size
    if (m_fileSize % TS_PACKET_SIZE != 0) 
    {
        std::cerr << "Warning: File size (" << m_fileSize << " bytes) is not a multiple of TS packet size (" 
                << TS_PACKET_SIZE << " bytes)" << std::endl;
    }
    
    return true;
}

void FileInputProvider::close() 
{
    if (m_isOpen && m_file.is_open()) 
    {
        m_file.close();
    }
    m_isOpen = false;
    m_processedPackets = 0;
}

size_t FileInputProvider::processNextBatch()
{
    if (!m_isOpen || !m_file.is_open() || !m_packetCallback) 
    {
        return 0;
    }
    
    // Calculate how many packets to read in this batch
    size_t remainingPackets = m_totalPackets - m_processedPackets;
    size_t packetsToRead = std::min(m_bufferSizeInPackets, remainingPackets);
    
    if (packetsToRead == 0) 
    {
        return 0; // No more packets to process
    }
    
    // Read a batch of packets
    size_t bytesToRead = packetsToRead * TS_PACKET_SIZE;
    m_file.read(reinterpret_cast<char*>(m_buffer.data()), bytesToRead);
    
    // Check how many bytes were actually read
    size_t bytesRead = m_file.gcount();
    size_t packetsRead = bytesRead / TS_PACKET_SIZE;
    
    if (packetsRead == 0) 
    {
        return 0; // Couldn't read any complete packets
    }
    
    // Process each packet in the buffer
    size_t processedCount = 0;
    std::vector<uint8_t> packet(TS_PACKET_SIZE);
    
    for (size_t i = 0; i < packetsRead; ++i) 
    {
        // Copy the packet from the buffer
        std::copy(
            m_buffer.data() + (i * TS_PACKET_SIZE),
            m_buffer.data() + ((i + 1) * TS_PACKET_SIZE),
            packet.begin()
        );
        
        // Verify TS sync byte (0x47)
        if (packet[0] != 0x47) 
        {
            std::cerr << "Warning: Invalid sync byte at packet #" 
                    << (m_processedPackets + i + 1) << ": 0x" 
                    << std::hex << static_cast<int>(packet[0]) << std::dec
                    << " (expected 0x47)" << std::endl;
            
            // Try to resync
            bool resynced = false;
            for (size_t j = 1; j < TS_PACKET_SIZE && (i * TS_PACKET_SIZE + j) < bytesRead; ++j) 
            {
                if (m_buffer[i * TS_PACKET_SIZE + j] == 0x47) 
                {
                    // Found a sync byte, adjust file position
                    std::streamoff offset = j - TS_PACKET_SIZE;
                    m_file.seekg(offset, std::ios::cur);
                    std::cerr << "Resynced after " << j << " bytes" << std::endl;
                    resynced = true;
                    break;
                }
            }
            
            if (!resynced) 
            {
                std::cerr << "Could not resync, skipping packet" << std::endl;
                // Skip this packet and continue
                continue;
            }
        }
        
        // Call the callback with this packet
        if (m_packetCallback(packet)) 
        {
            processedCount++;
        }
        else
        {
            // Callback requested stop
            break;
        }
    }
    
    m_processedPackets += processedCount;
    return processedCount;
}

bool FileInputProvider::hasMorePackets() const 
{
    if (!m_isOpen || !m_file.is_open()) 
    {
        return false;
    }
    
    return m_processedPackets < m_totalPackets;
}

bool FileInputProvider::reset()
{
    if (!m_isOpen || !m_file.is_open()) 
    {
        return false;
    }
    
    m_file.clear(); // Clear EOF and error flags
    m_file.seekg(0, std::ios::beg);
    m_processedPackets = 0;
    return true;
}

std::string FileInputProvider::getSourceDescription() const
{
    return "File: " + m_filename + " (" + 
           std::to_string(m_totalPackets) + " packets, " +
           std::to_string(m_processedPackets) + " processed)";
}

// Get the total number of packets in the file
size_t FileInputProvider::getTotalPackets() const
{
    return m_totalPackets;
}

// Get the number of packets processed so far
size_t FileInputProvider::getProcessedPackets() const
{
    return m_processedPackets;
}

std::unique_ptr<InputProvider> createInputProvider(const std::string& source, size_t bufferSize)
{
    // Check if source is a file path (basic check)
    if (source.find("://") == std::string::npos) 
    {
        return std::make_unique<FileInputProvider>(source, bufferSize);
    }
    
    // Future extensions can handle other source types
    // e.g., "udp://..." or "memory://..."
    throw std::runtime_error("Unsupported input source: " + source);
}



