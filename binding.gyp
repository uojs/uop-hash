{
    "targets": [
        {
            "target_name": "hash",
            "sources": [ "src/hash.cc" ],
            "include_dirs" : [
                "<!(node -e \"require('nan')\")"
            ]
        }
    ]
}
