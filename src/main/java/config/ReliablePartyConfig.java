package config;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.dataformat.yaml.YAMLFactory;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Map;

public class ReliablePartyConfig extends BaseConfig {

    private String id;

    public ReliablePartyConfig(@JsonProperty(value = "address", required = true) String address,
                               @JsonProperty(value = "port", required = true) Integer port,
                               @JsonProperty(value = "parties", required = true) ArrayList<Map> parties,
                               @JsonProperty(value = "id", required = true) String id) {
        super(address, port, parties);
        this.id = id;
    }

    public static ReliablePartyConfig load(String path) throws IOException {
        ObjectMapper mapper = new ObjectMapper(new YAMLFactory());
        return mapper.readValue(new File(path), ReliablePartyConfig.class);
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

}
