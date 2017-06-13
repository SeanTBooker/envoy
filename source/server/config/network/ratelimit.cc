#include "server/config/network/ratelimit.h"

#include <chrono>
#include <string>

#include "envoy/network/connection.h"

#include "common/filter/ratelimit.h"

#include "server/configuration_impl.h"

namespace Envoy {
namespace Server {
namespace Configuration {

NetworkFilterFactoryCb RateLimitConfigFactory::createFilterFactory(const Json::Object& json_config,
                                                                   FactoryContext& context) {
  RateLimit::TcpFilter::ConfigSharedPtr config(
      new RateLimit::TcpFilter::Config(json_config, context.scope(), context.runtime()));
  return [config, &context](Network::FilterManager& filter_manager) -> void {
    filter_manager.addReadFilter(Network::ReadFilterSharedPtr{new RateLimit::TcpFilter::Instance(
        config, context.rateLimitClient(Optional<std::chrono::milliseconds>()))});
  };
}

/**
 * Static registration for the rate limit filter. @see RegisterNamedNetworkFilterConfigFactory.
 */
static RegisterNamedNetworkFilterConfigFactory<RateLimitConfigFactory> registered_;

} // Configuration
} // Server
} // Envoy
