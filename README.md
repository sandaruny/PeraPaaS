# PeraPaaS

This is a low latency PaaS container for high end switching

Cloud computing is a form of standardized IT based capability, such as Internet-based services, software, 
or IT infrastructure offered by a service provider that is accessible via Internet protocols from any computer.
Cloud Infrastructure is always available and scales automatically to adjust to demand, is either pay-per-use or advertising based,
has Web or Programmatic based control interfaces, and enables full customer self- service. 

When an application deployed in a cloud environment the customer have to pay for every node that is added when auto scaling to
handle the request load. 

When messages with specific light weight tasks such as retrieving a small dataset from huge data even though the message logic 
and the message content is low we have to deal with a multiple range of data traffics that balances the requests. If a 
lightweight retrieval and an uploading of an image are both processed by a same application instance the expansion of node 
happens momentarily which causes the customer more money.

Our vision is to build a PaaS container that process the lightweight messages in a specific way that the expansion is minimum. 
This platform will support quick message switching with a reliable architecture to support minimum expansion.
