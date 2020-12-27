QByteArray arr;
    Message mes(arr);
    mes.type = MessageType::MOVE;
    mes.room_id = 10;

    std::ostringstream ss;
    cereal::BinaryOutputArchive oarchive(ss);
    oarchive(mes);

    std::istringstream ss2(ss.str());
    cereal::BinaryInputArchive iarchive(ss2);
    Message mes2(arr);
    iarchive(mes2);
    std::cout << mes2.room_id << std::endl;

