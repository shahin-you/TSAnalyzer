#ifndef TSA_FILE_INPUT_PROVIDER_H
#define TSA_FILE_INPUT_PROVIDER_H

#include "inputProvider.h"
#include <iostream>
#include <fstream>

/**
 * @brief Implementation of InputProvider for reading TS packets from a file
 */
class FileInputProvider : public InputProvider 
{
private:
    std::string m_filename;
    std::ifstream m_file;
    bool m_isOpen;
    std::streampos m_fileSize;
    size_t m_totalPackets;
    size_t m_processedPackets;
    
    // Buffer for reading multiple packets
    std::vector<uint8_t> m_buffer;
    size_t m_bufferSizeInPackets;
    
    // Callback for packet processing
    TSPacketCallback m_packetCallback;
    
public:
    explicit FileInputProvider(const std::string& filename, size_t bufferSizeInPackets = DEFAULT_BUFFER_SIZE);
    
    ~FileInputProvider() override 
    {
        close();
    }
    
    void setPacketCallback(TSPacketCallback callback) override;
    
    bool open() override;
    
    void close() override;
    
    size_t processNextBatch() override;
    
    bool hasMorePackets() const override;
    
    bool reset() override;
    
    std::string getSourceDescription() const override;
    
    // Get the total number of packets in the file
    size_t getTotalPackets() const;
    
    // Get the number of packets processed so far
    size_t getProcessedPackets() const;
};

// Factory function to create an appropriate input provider based on source type
std::unique_ptr<InputProvider> createInputProvider(const std::string& source, size_t bufferSize = DEFAULT_BUFFER_SIZE);

#endif